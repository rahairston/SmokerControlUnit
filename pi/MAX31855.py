import RPi.GPIO as GPIO
import spidev
from time import sleep

delay_micro = 0.00001
retries = 64

class MAX31855:

    sclk_pin = -1
    cs_pin = -1
    so_pin = -1
    faultMask = 0x07
    
    # Fault codes
    FAULT_NONE = 0x00
    FAULT_OPEN = 0x01
    FAULT_SHORT_GND = 0x02
    FAULT_SHORT_VCC = 0x04
    FAULT_ALL = 0x07


    def __init__(self, sclk_pin, cs_pin, so_pin):
        self.sclk_pin = sclk_pin
        self.cs_pin = cs_pin
        self.so_pin = so_pin
        GPIO.setwarnings(False)			#disable warnings
        GPIO.setmode(GPIO.BOARD)		#set pin numbering system
        GPIO.setup(sclk_pin,GPIO.OUT)
        GPIO.setup(cs_pin,GPIO.OUT)
        GPIO.setup(so_pin,GPIO.IN)

        GPIO.output(cs_pin, 1)
        # We only have SPI bus 0 available to us on the Pi
        bus = 0

        #Device is the chip select pin. Set to 0 or 1, depending on the connections
        device = 1

        # Enable SPI
        spi = spidev.SpiDev()

        # Open a connection to a specific bus and device (chip select pin)
        spi.open(bus, device)

        # Set SPI speed and mode
        spi.max_speed_hz = 500000
        spi.mode = 0

    def setFaultChecks(self, faults):
        self.faultMask = faults & 0x07

    def spiread(self):
        # GPIO.output(self.cs_pin, 0)
        b = spidev.readbytes(1)
        d = b[0]
        d <<= 8
        d |= b[1]
        d <<= 8
        d |= b[2]
        d <<= 8
        d |= b[3]

        # GPIO.output(self.cs_pin, 1)

        return d

    def readError(self):
        return self.spiread() & 0x7

    def readRaw(self):
        for i in range(0, retries):
            self.spiread()
        
    def readInternal(self):
        v = self.spiread()

        v >>= 4

        internal = v & 0x7FF
        # check sign bit!
        if v & 0x800:
            # Convert to negative value by extending sign and casting to signed type.
             tmp = 0xF800 | (v & 0x7FF)
            internal = tmp

        internal *= 0.0625; # LSB = 0.0625 degrees
        
        return internal
    
    def readCelsius(self):
        v = self.spiread()

        if v & self.faultMask:
            # uh oh, a serious problem!
            return None
        
        if v & 0x80000000:
            # Negative value, drop the lower 18 bits and explicitly extend sign bits.
            v = 0xFFFFC000 | ((v >> 18) & 0x00003FFF)
        else:
            # Positive value, just drop the lower 18 bits.
            v >>= 18

        return v * 0.25
    
    def readFahrenheit(self):
        f = self.readCelsius()
        f *= 9.0
        f /= 5.0
        f += 32
        return f


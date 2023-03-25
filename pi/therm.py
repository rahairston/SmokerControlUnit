import RPi.GPIO as GPIO
from time import sleep

sclk_pin = 7
cs_pin = 11
so_pin = 13
GPIO.setwarnings(False)			#disable warnings
GPIO.setmode(GPIO.BOARD)		#set pin numbering system
GPIO.setup(sclk_pin,GPIO.OUT)
GPIO.setup(cs_pin,GPIO.OUT)
GPIO.setup(so_pin,GPIO.IN)

GPIO.output(cs_pin, 1)

delay_micro = 0.00001

sleep(2)


def spiread():
    
    b = 0
    
    for i in range(7,-1,-1):
        GPIO.output(sclk_pin, 0)
        sleep(delay_micro)
        if (GPIO.input(so_pin)):
            b |= (1 << i)
        
        GPIO.output(sclk_pin, 1)
        sleep(delay_micro)
        
    return b


while(True):
    GPIO.output(cs_pin, 0)
    sleep(delay_micro)
    v = spiread()
    v <<= 8
    v |= spiread()
    GPIO.output(cs_pin, 1)

    if (v & 0x4):
        print("NaN")
    
    v >>= 3
    celsius = v * 0.25
    fahrenheit = celsius * 9.0 / 5.0 + 32
    print(celsius, fahrenheit)

    sleep(3)
    
    


from MAX31855 import MAX31855
from time import sleep
import RPi.GPIO as GPIO

sclk_pin = 7
cs_pin = 11
so_pin = 13

GPIO.setwarnings(False)			#disable warnings
GPIO.setmode(GPIO.BOARD)		#set pin numbering system

sleep(2)

MAX31855 = MAX31855(sclk_pin, cs_pin, so_pin)

sleep(2)

while(True):
    print(MAX31855.readInternal())

    c = MAX31855.readCelsius()
    if c == None:
        print("ERROR:")
        e = thermocouple.readError()
        if (e & MAX31855_FAULT_OPEN):
            print("FAULT: Thermocouple is open - no connections.")
        if (e & MAX31855_FAULT_SHORT_GND):
            print("FAULT: Thermocouple is short-circuited to GND.")
        if (e & MAX31855_FAULT_SHORT_VCC):
            print("FAULT: Thermocouple is short-circuited to VCC.")
    else:
        print(f"Temp is: {c}")

    sleep(1)
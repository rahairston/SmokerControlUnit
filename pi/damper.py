import RPi.GPIO as GPIO
from time import sleep

pwm_pin = 18
frequency = 48 
open_duty = 10
close_duty = open_duty / 2
fully_closed_position = 0
fully_open_position = 20
min_time = 0.016 # found via testing to be smallest registered time for movement
max_time = 0.23 # time from full open to full close
base_increment = (max_time - min_time) / fully_open_position
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

class Damper:
    def __init__(self, pin=pwm_pin, frequency=frequency):
        GPIO.setup(pin,GPIO.OUT)
        self.pwm = GPIO.PWM(pin,frequency)
        self.pwm.start(0)
        self.position = fully_closed_position
    
    # moves arm to fully open, then closed
    def setup(self):
        self.setFullyOpen()
        sleep(1)
        self.setFullyClosed()
        self.pwm.ChangeDutyCycle(0)

    # temp diff = current - target
    def changePosition(self, temp_diff, previous_temp_diff, polling_time):
        self.pwm.ChangeDutyCycle(0)
        # too cold -> close damper
        if temp_diff < 0:
            if self.position >= fully_closed_position: # damper in max close position, can't make it hotter
                self.position = fully_closed_position
                return
            self.pwm.ChangeDutyCycle(close_duty)
        elif temp_diff > 0: # too hot -> open damper
            if self.position >= fully_open_position: # damper in max open position, it'll cool down eventually... right?
                self.position = fully_open_position
                return
            self.pwm.ChangeDutyCycle(open_duty)
        sleep_time = 0
        sleep(sleep_time)
        # always set back to zero
        self.pwm.ChangeDutyCycle(0)

    def setFullyOpen(self):
        self.pwm.ChangeDutyCycle(open_duty)
        sleep(.5)
        self.pwm.ChangeDutyCycle(0)

    # 0.23 is time from fully open to close through testing
    # However if we don't know the current position we can overshoot, then close back
    # by "opening" for 0.08s (also through testing)
    def setFullyClosed(self):
        self.pwm.ChangeDutyCycle(close_duty)
        sleep(.5)
        self.pwm.ChangeDutyCycle(open_duty)
        sleep(.08)
        self.pwm.ChangeDutyCycle(0)


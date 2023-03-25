import RPi.GPIO as GPIO
from time import sleep

pwm_pin = 12				# PWM pin connected to LED
GPIO.setwarnings(False)			#disable warnings
GPIO.setmode(GPIO.BOARD)		#set pin numbering system
GPIO.setup(pwm_pin,GPIO.OUT)
frequency = 48 
pi_pwm = GPIO.PWM(pwm_pin,frequency)		#create PWM instance with frequency
pi_pwm.start(0)		#start PWM of required Duty Cycle
sleep(1)

open_duty = 10
close_duty = open_duty / 2

def startup():
    pi_pwm.ChangeDutyCycle(open_duty)
    sleep(.45)
    pi_pwm.ChangeDutyCycle(0)
    sleep(2)
    pi_pwm.ChangeDutyCycle(close_duty)
    #pi_pwm.ChangeFrequency(48)
    sleep(.38)
    pi_pwm.ChangeDutyCycle(0)
    sleep(2)

while True:
    startup()
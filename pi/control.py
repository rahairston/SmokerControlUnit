from damper import Damper
import Adafruit_MAX31855.MAX31855 as MAX31855
from time import sleep

CLK = 4 # pin 7
CS  = 17 # pin 11
DO  = 27 # pin 13

class ControlUnit:
    def __init__(self, polling_time):
        self.damper = Damper()
        self.sensor = MAX31855.MAX31855(CLK, CS, DO)
        self.polling_time = polling_time

    def setup(self):
        self.damper.setup()
        sleep(0.5)
        self.target_temp = self.readTempF()
        self.previous_temp_diff = self.readTempF() - self.target_temp

    # Because I'm a US heathen
    def readTempF(self):
        return self.sensor.readTempC() * 9.0 / 5.0 + 32.0
    
    def updateTarget(self, new_temp):
        self.target_temp = new_temp
    
    def updatePollingTime(self, new_time):
        self.polling_time = new_time

    def updateDamper(self):
        temp_diff = self.readTempF() - self.target_temp
        self.damper.changePosition(temp_diff, self.previous_temp_diff, self.polling_time)
        self.previous_temp_diff = temp_diff
    

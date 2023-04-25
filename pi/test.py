from control import ControlUnit
from time import sleep

unit = ControlUnit(5)

unit.setup()

count = 0
while count < 10:
	print(unit.readTempF())
	print(unit.sensor.readState())
	sleep(1.5)
	count += 1

from control import ControlUnit
from time import sleep

unit = ControlUnit(5)

unit.setup()

unit.damper.testPosition(0.24)
sleep(4)

#for i in range(1, 50):
#	sleep_time = (i / 1000.0) + 0.016
#	
#	print(f"Sleeping for {sleep_time} seconds")
#	unit.damper.testPosition(sleep_time, i % 2 == 0)
#	sleep(2)


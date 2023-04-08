# Python Control Unit
This section of code is for use on a raspberry pi with Python3

## Additional Prerequisites
(see top level readme for standard prereqs)
- Raspbery PI 3B or later
- Python 3.9 or later
- sudo access (for setup script)

##  PI Setup

### Damper
3 pins to be used:
- 5V pin
- GND pin
- 1 of the 2 PWM0 pins 
    - **note**: the Thermalcouple code from Adafruit forces the board mode into BCM, so make sure you are using the GPIO number and not the pin number e.g. GPIO 18 -> 18 in code for the `pwm_pin`

### Thermocouple
5 pins to be used:
- 3.3V pin
- GND pin
- 1 pin for SCLK (usually pin 7/GPIO4 is the CLK pin)
- 2 general purpose pins
    - 1 for CS (chip select) from MAX31855
    - 1 for DO from MAX31855

## Disclaimers

Using AdaFruit Python MAX31855 library from 
https://github.com/adafruit/Adafruit_Python_MAX31855
Though it may be deprecated, the newer library does not provide a software SPI solution like this one does

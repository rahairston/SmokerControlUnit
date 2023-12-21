#include "Adafruit_MAX31855.h"
#include <SPI.h>
#include "web.h"

int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;

Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int targetTemp = 255;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }
  
  web::setup();

}

void loop() {
  web::print_page();
}

double getTemperature() {
  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Thermocouple fault(s) detected!");
    uint8_t e = thermocouple.readError();
    if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
  } else {
    return thermocouple.readFahrenheit();
  }
}
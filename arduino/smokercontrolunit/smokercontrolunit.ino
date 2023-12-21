#include "arduino_secrets.h"
#include "Adafruit_MAX31855.h"
#include <SPI.h>
#include "web.h"

int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;

WiFiClient client;
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

char ssid[] = SECRET_SSID;             //  your network SSID (name) between the " "
char pass[] = SECRET_PASS;      // your network password between the " "
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
  client = web::get_server();

  if (client) {
    printWEB();
  }
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

void printWEB() {

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
            //create the buttons
            client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED off<br><br>");
            
            double temp = getTemperature();
            client.print("Current Temperature: ");
            client.print(temp);

            client.print("<br>Target Temperature: ");
            client.print(targetTemp);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H")) {
          targetTemp += 1;       
        }
        if (currentLine.endsWith("GET /L")) {
          targetTemp -= 1;
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
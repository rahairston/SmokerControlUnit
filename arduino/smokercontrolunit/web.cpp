#include "web.h"

int status = WL_IDLE_STATUS;      //connection status

namespace web {

  WiFiServer server(80);  //server socket

  WiFiClient client = server.available();

  void setup() {
    enable_WiFi();
    access_point_mode();

    server.begin();
    printWifiStatus();
  }

  void access_point_mode() {
    status = WiFi.beginAP(AP_SSID);
    if (status != WL_AP_LISTENING) {
      Serial.println("Creating access point failed");
      // don't continue
      while (true);
    }
  }

  void enable_WiFi() {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      // don't continue
      while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0") {
      Serial.println("Please upgrade the firmware");
    }

    WiFi.setHostname("smokerunit");
  }

  void connect_WiFi(char* ssid, char*  pass) {
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);

      // wait 10 seconds for connection:
      delay(10000);
    }
  }

  WiFiClient get_server() {
    return server.available();
  }

  void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
  }
}
#pragma once

#include <WiFiNINA.h>
#include "Arduino.h"

#define AP_SSID "SmokerCU"

namespace web {
  void setup();
  WiFiClient get_server();
  void enable_WiFi();
  void connect_WiFi(char* ssid, char* pass);
  void printWifiStatus();

  void access_point_mode();

  void web_mode();
}
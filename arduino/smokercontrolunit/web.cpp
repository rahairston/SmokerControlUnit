#include "web.h"

int status = WL_IDLE_STATUS;      //connection status
int mode = 0;
String line_check = "GET /submit?";

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

  void connect_WiFi(char* ssid, char* pass) {
    // End AP Mode
    // WiFi.end();
    // delay(2000);
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
      mode = 1;

      // wait 10 seconds for connection:
      delay(5000);
    }

    printWifiStatus();
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

  void print_page() {
    client = server.available();

    if (status != WiFi.status()) {
    // it has changed update the variable
      status = WiFi.status();

      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
      }
    }

    if (client) {
      Serial.println("client avail");
      if (mode == 0) {
        print_access_page();
      } else {
        print_web_page();
      }
    }
  }

  void print_access_page() {
    if (client) {                             // if you get a client,
      Serial.println("new client");           // print a message out the serial port
      String final_params = "";
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {

              if (!final_params.equals("")) {
                int ssid_index_start = final_params.indexOf("=") + 1;
                int ssid_index_end = final_params.indexOf("&");
                int pass_index_start = final_params.lastIndexOf("=") + 1;
                int pass_index_end = final_params.lastIndexOf(" ");
                String ssid = final_params.substring(ssid_index_start, ssid_index_end);
                String password = final_params.substring(pass_index_start, pass_index_end);
                if (!ssid.equals("") && !password.equals("")) {
                  char ssid_buf[ssid.length()];
                  char pass_buf[password.length()];
                  ssid.toCharArray(ssid_buf, ssid.length() + 1);
                  password.toCharArray(pass_buf, password.length() + 1);
                  connect_WiFi(ssid_buf, pass_buf);
                }
              }

              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              //create the buttons
              client.print("<form action='/submit'>");
              client.print("<label style='font-size:4em;' for='ssid'>SSID:</label><br>");
              client.print("<input style='font-size:4em;' type='text' id='ssid' name='ssid' value=''><br>");
              client.print("<label style='font-size:4em;' for='pass'>Password:</label><br>");
              client.print("<input style='font-size:4em;' type='password' id='pass' name='pass' value=''><br><br>");
              client.print("<a href='http://smokerunit'><input style='font-size:4em;' type='submit' value='Submit'></a>");
              client.print("</form>");

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

          if (currentLine.indexOf(line_check) != -1) {
            int index = currentLine.indexOf(line_check);
            final_params = currentLine.substring(index + line_check.length());
          }
        }
      }

      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
  }

  void print_web_page() {
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
              
              // double temp = getTemperature();
              // client.print("Current Temperature: ");
              // client.print(temp);

              // client.print("<br>Target Temperature: ");
              // client.print(targetTemp);

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

          // if (currentLine.endsWith("GET /H")) {
          //   targetTemp += 1;       
          // }
          // if (currentLine.endsWith("GET /L")) {
          //   targetTemp -= 1;
          // }

        }
      }
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
  }
}
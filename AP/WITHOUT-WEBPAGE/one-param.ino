#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int ledPin = 2; // Pin for the LED
boolean ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/toggle", HTTP_POST, [](){
    String command = server.arg("command"); // Get the value of the "command" parameter

    if (command == "on") {
      digitalWrite(ledPin, HIGH);
      ledState = true;
    } else if (command == "off") {
      digitalWrite(ledPin, LOW);
      ledState = false;
    }

    server.send(200);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

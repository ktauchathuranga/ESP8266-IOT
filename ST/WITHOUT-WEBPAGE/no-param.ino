#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "YourWiFiSSID";   // Replace with your WiFi network SSID
const char* password = "YourWiFiPassword"; // Replace with your WiFi network password

const int ledPin = 2; // Pin for the LED
boolean ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.on("/toggle", HTTP_POST, [](){
    digitalWrite(ledPin, !ledState); // Toggle LED state
    ledState = !ledState;
    server.send(200);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

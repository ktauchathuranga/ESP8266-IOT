#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int ledPin = 2; // Pin for the LED
boolean ledState = false;
int ledBrightness = 0; // Variable to store LED brightness

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/control", HTTP_POST, [](){
    String command = server.arg("command");       // Get the value of the "command" parameter
    String brightness = server.arg("brightness"); // Get the value of the "brightness" parameter

    if (command == "on") {
      digitalWrite(ledPin, HIGH);
      ledState = true;
    } else if (command == "off") {
      digitalWrite(ledPin, LOW);
      ledState = false;
    }

    if (brightness.length() > 0) {
      ledBrightness = brightness.toInt(); // Convert brightness value to an integer
      analogWrite(ledPin, ledBrightness); // Set LED brightness using PWM
    }

    server.send(200);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

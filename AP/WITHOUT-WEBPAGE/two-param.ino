#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int ledPin = 2; // Pin for the LED
boolean ledState = false;
int ledBrightness = 0; // Variable to store LED brightness

IPAddress localIP(192, 168, 1, 1);  // Set your desired IP address
IPAddress gateway(192, 168, 1, 1); // Set your gateway IP address
IPAddress subnet(255, 255, 255, 0); // Set your subnet mask

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  
  // Connect to WiFi network
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

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

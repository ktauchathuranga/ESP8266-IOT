#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int sensorPin1 = A0; // Analog pin for the first sensor
const int sensorPin2 = A1; // Analog pin for the second sensor

IPAddress localIP(192, 168, 1, 1);  // Set your desired IP address
IPAddress gateway(192, 168, 1, 1); // Set your gateway IP address
IPAddress subnet(255, 255, 255, 0); // Set your subnet mask

void setup() {
  Serial.begin(115200);
  delay(10);
  
  analogReference(DEFAULT);
  
  // Connect to WiFi network
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/get-sensor", HTTP_POST, [](){
    String response = "";

    if (server.hasArg("sensor1")) {
      if (server.arg("sensor1").toInt() == 1) {
        response += "Sensor 1: " + String(analogRead(sensorPin1)) + "\n";
      } else {
        response += "Sensor 1: None\n";
      }
    }

    if (server.hasArg("sensor2")) {
      if (server.arg("sensor2").toInt() == 1) {
        response += "Sensor 2: " + String(analogRead(sensorPin2)) + "\n";
      } else {
        response += "Sensor 2: None\n";
      }
    }

    if (response.isEmpty()) {
      response = "No sensor value requested.";
    }

    server.send(200, "text/plain", response);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

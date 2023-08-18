#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int ledPin = 2; // Pin for the LED
boolean ledState = false;

IPAddress localIP(192, 168, 1, 1);  // Set your desired IP address
IPAddress gateway(192, 168, 1, 1); // Set your gateway IP address
IPAddress subnet(255, 255, 255, 0); // Set your subnet mask

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](void){
    String html = "<html><body>";
    html += "<h1>Welcome to ESP8266 Web Server</h1>";
    html += "<button id=\"toggleButton\">Toggle LED</button>";
    html += "<script>";
    html += "document.getElementById('toggleButton').addEventListener('click', function() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('POST', '/toggle', true);";
    html += "  xhr.send();";
    html += "});";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

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

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "ESP8266_AP";
const char* password = "password123";

const int ledPin = 2; // Pin for the LED
const int sensorPin = A0; // Analog pin for the sensor

boolean ledState = false;

IPAddress localIP(192, 168, 1, 1);  // Set your desired IP address
IPAddress gateway(192, 168, 1, 1); // Set your gateway IP address
IPAddress subnet(255, 255, 255, 0); // Set your subnet mask

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  analogReference(DEFAULT);
  
  // Connect to WiFi network
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](void){
    String html = "<html><body>";
    html += "<h1>Welcome to ESP8266 Web Server</h1>";
    html += "<p>Sensor Value: <span id=\"sensorValue\">--</span></p>";
    html += "<button id=\"toggleButton\">Toggle LED</button>";
    html += "<script>";
    html += "document.getElementById('toggleButton').addEventListener('click', function() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('POST', '/toggle', true);";
    html += "  xhr.send();";
    html += "});";
    html += "setInterval(function() {";
    html += "  var sensorXhr = new XMLHttpRequest();";
    html += "  sensorXhr.onload = function() {"; // Change here
    html += "    if (sensorXhr.status === 200) {"; // Also update this line
    html += "      document.getElementById('sensorValue').textContent = sensorXhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  sensorXhr.open('GET', '/sensor', true);";
    html += "  sensorXhr.send();";
    html += "}, 1000);"; // Update every 1 second
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/toggle", HTTP_POST, [](){
    digitalWrite(ledPin, !ledState); // Toggle LED state
    ledState = !ledState;
    server.send(200);
  });

  server.on("/sensor", HTTP_GET, [](){
    int sensorValue = analogRead(sensorPin);
    server.send(200, "text/plain", String(sensorValue));
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

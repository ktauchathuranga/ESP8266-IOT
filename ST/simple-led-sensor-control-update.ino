#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "YourWiFiSSID";   // Replace with your WiFi network SSID
const char* password = "YourWiFiPassword"; // Replace with your WiFi network password

const int ledPin = 2; // Pin for the LED
const int sensorPin = A0; // Analog pin for the sensor

boolean ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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
    html += "  sensorXhr.onreadystatechange = function() {";
    html += "    if (sensorXhr.readyState === 4 && sensorXhr.status === 200) {";
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
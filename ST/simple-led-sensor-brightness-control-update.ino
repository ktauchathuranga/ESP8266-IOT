#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "YourWiFiSSID";   // Replace with your WiFi network SSID
const char* password = "YourWiFiPassword"; // Replace with your WiFi network password

const int ledPin = 2; // Pin for the LED
const int sensorPin = A0; // Analog pin for the sensor

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
    html += "<input type='range' id='brightnessSlider' min='0' max='255' value='128'>";
    html += "<script>";
    html += "document.getElementById('brightnessSlider').addEventListener('input', function() {";
    html += "  var brightnessValue = document.getElementById('brightnessSlider').value;";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('POST', '/brightness', true);";
    html += "  xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');";
    html += "  xhr.send('value=' + brightnessValue);";
    html += "});";
    html += "setInterval(function() {";
    html += "  var sensorXhr = new XMLHttpRequest();";
    html += "  sensorXhr.onload = function() {";
    html += "    if (sensorXhr.status === 200) {";
    html += "      document.getElementById('sensorValue').textContent = sensorXhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  sensorXhr.open('GET', '/sensor', true);";
    html += "  sensorXhr.send();";
    html += "}, 1000);";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/sensor", HTTP_GET, [](){
    int sensorValue = analogRead(sensorPin);
    server.send(200, "text/plain", String(sensorValue));
  });

  server.on("/brightness", HTTP_POST, [](){
    int brightnessValue = server.arg("value").toInt();
    analogWrite(ledPin, brightnessValue); // Set LED brightness
    server.send(200);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // You can add any other code or features here if needed
}

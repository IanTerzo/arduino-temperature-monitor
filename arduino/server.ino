#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80); // Create a web server on port 80

String WIFI_SSID = "123";
String WIFI_PASS = "123";
String WEBSITE_IP = "http://123.123.12.12:8000";

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  
  // Route for handling the root page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "Hello, ESP8266!");
    
  });

  server.on("/load", HTTP_GET, []() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float hic = dht.computeHeatIndex(t, h, false);

    server.sendHeader("Access-Control-Allow-Origin", "http://192.168.56.44:8000");

    StaticJsonDocument<200> doc;

    doc["temp"] = String(t);
    doc["humidity"] = String(h);
    doc["hic"] = dht.computeHeatIndex(t, h, false);

    String jsonString;
    serializeJson(doc, jsonString);

    Serial.println(jsonString);
  
    server.send(200,  "text/plain", jsonString);
  });
  
  // Start the server
  server.begin();
}

void loop() {
  server.handleClient(); // Handle client requests
}

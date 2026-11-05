#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>

const char *ssid = "";
const char *password = "";

ESP8266WebServer server(80);

unsigned long previousMillis = 0;
const long interval = 5000; // Інтервал виконання фонового завдання (5 секунд)

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("\nBooting non-blocking hub...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected success!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []()
            { server.send(200, "text/html", "<h1>ESP8266 Non-Blocking Hub</h1><p>Status: Active</p><p><a href='/update'>Go to OTA Update</a></p>"); });

  ElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server and OTA started.");
}

void loop()
{
  server.handleClient();
  ElegantOTA.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    Serial.println("Background task: System is running smoothly (non-blocking).");
  }
}
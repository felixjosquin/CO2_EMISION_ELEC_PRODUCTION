#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "secret.h"
#include "apiRTE.h"
#include "productValues.h"

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Start to connect");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(100);
  }
  getToken();
  getData();
  printListProductObjects();
}

void loop() {
}
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "secret.h"
#include "requestAPI.h"
#include "productValues.h"
#include "date.h"
#include "leds.h"



void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Start to connect");
  initColors();
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (!WiFi.isConnected()) {
    Serial.print(".");
    waitStep();
    delay(100);
  }
  Serial.println(" sucess !!");
  getToken();
  getDateTime();
  getData();
  printListProductObjects();
  Serial.println(computeCO2());
}

void loop() {

}
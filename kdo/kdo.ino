#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "secret.h"
#include "requestAPI.h"
#include "productValues.h"
#include "date.h"
#include "leds.h"

#define TIME_REFRESH 10

void setup() {
  Serial.begin(9600);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  initColors();

  tryAndRetry(REQUEST_TOKEN);
  tryAndRetry(REQUEST_DATE);
}

void loop() {
  printDatetime();
  tryAndRetry(REQUEST_MIX_ENERGETIQUE);
  Serial.println();
  printListProductObjects();
  Serial.println();
  Serial.print("CO² : ");
  Serial.println(computeCO2());
  showCo2(computeCO2());
  delay(TIME_REFRESH * 60000);
  addTime(TIME_REFRESH);
}
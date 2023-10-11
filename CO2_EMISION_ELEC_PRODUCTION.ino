#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "src/secret.h"
#include "src/requestAPI.h"
#include "src/productValues.h"
#include "src/date.h"
#include "src/leds.h"

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
  printListProductObjects();
  
  Serial.print("Impact carbon : ");
  Serial.print(computeCO2());
  Serial.println(" gCO²/kWh");

  showCo2(computeCO2());
  delay(TIME_REFRESH * 60000);
  addTime(TIME_REFRESH);
}
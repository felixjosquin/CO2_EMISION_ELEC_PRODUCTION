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
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  initColors();
  
  tryAndRetry(REQUEST_TOKEN);
  tryAndRetry(REQUEST_DATE);
  tryAndRetry(REQUEST_MIX_ENERGETIQUE);

  printListProductObjects();
  Serial.println(computeCO2());
  showCo2(computeCO2());
}

void loop() {

}
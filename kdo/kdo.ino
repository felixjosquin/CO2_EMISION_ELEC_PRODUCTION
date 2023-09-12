#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "secret.h"
#include "requestAPI.h"
#include "productValues.h"
#include "date.h"

// void setup(){
//   Serial.begin(9600);
//   Serial.println();
//   Serial.println("Start to connect");
//   DATE date = getDateFromString("\"2023-09-12T13:26:01.353796-02:00\"");
//   date.display();
// }

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
  Serial.println(" sucess !!");
  getToken();
  getData();
  printListProductObjects();
}

void loop() {
}
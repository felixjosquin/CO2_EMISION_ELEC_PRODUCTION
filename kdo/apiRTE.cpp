#include "apiRTE.h"
#include "secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>

WiFiClientSecure client;
HTTPClient http;
String token;


String getToken() {
  client.setInsecure();
  http.begin(client, "https://digital.iservices.rte-france.com/token/oauth");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", secret_api_rte);
  int httpCode = http.POST("");
  if (httpCode == 200) {
    String reponse = http.getString();
    reponse.remove(0, reponse.indexOf("access_token"));
    token = reponse.substring(reponse.indexOf(": \"") + 3, reponse.indexOf("\","));
    // StaticJsonDocument<192> tokenDoc;
    // deserializeJson(tokenDoc, http.getString());
    // const char* accessToken = tokenDoc["access_token"];
    // token = String(accessToken);
  }
  http.end();
  return token;
}
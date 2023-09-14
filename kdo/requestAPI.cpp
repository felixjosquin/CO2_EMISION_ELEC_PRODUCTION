#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>

#include "requestAPI.h"
#include "secret.h"
#include "productValues.h"
#include "useJSON.h"
#include "date.h"

WiFiClientSecure* client = new WiFiClientSecure;
HTTPClient* http = new HTTPClient;
String token;

String getProductType(String* headerProductionType);
String getProductValue(String* productValueObject);
String getStartDate(String* productValueObject);
char getClearChar();

void getToken() {
  client->setInsecure();
  http->begin(*client, "https://digital.iservices.rte-france.com/token/oauth");
  http->addHeader("Content-Type", "application/x-www-form-urlencoded");
  http->addHeader("Authorization", RTE_PASS);
  int httpCode = http->POST("");
  Serial.print("Request RTE Token : ");
  Serial.println(httpCode);
  if (httpCode == 200) {
    String reponse = http->getString();
    reponse.remove(0, reponse.indexOf("access_token"));
    token = reponse.substring(reponse.indexOf(": \"") + 3, reponse.indexOf("\","));
  }
  http->end();
}

bool getData() {
  client->setInsecure();
  String header = "Bearer " + token;
  http->begin(*client, "https://digital.iservices.rte-france.com/open_api/actual_generation/v1/generation_mix_15min_time_scale?production_subtype=TOTAL&start_date=" + getStringToday() + "&end_date=" + getStringForNextDay());
  http->addHeader("Authorization", header);
  int httpCode = http->GET();
  Serial.print("Request RTE Mix " + getStringToday() + " -> " + getStringForNextDay() + ": ");
  Serial.println(httpCode);
  if (httpCode == 200) {
    client->readStringUntil('{');
    client->readStringUntil('{');
    char productionTypeChar;
    String productionType;
    do {
      String headerProductionType = client->readStringUntil('[');
      productionType = getProductType(&headerProductionType);
      char productValueChar;
      String productValue;
      String startDateString;
      do {
        String productValueObject = client->readStringUntil('}').substring(1);
        startDateString = getStartDate(&productValueObject);
        if (startDateString.indexOf(getStringStartDate()) != -1) {
          productValue = getProductValue(&productValueObject);
        }
        productValueChar = getClearChar();
      } while (productValueChar == ',');

      updateValue(productionType, productValue);

      getClearChar();
      productionTypeChar = getClearChar();
    } while (productionTypeChar == ',');
  }
  http->end();
  return true;
}

bool getDateTime() {
  client->setInsecure();
  http->begin(*client, "https://worldtimeapi.org/api/timezone/Europe/Paris");
  int httpCode = http->GET();
  Serial.print("Request Datetime : ");
  Serial.println(httpCode);
  if (httpCode == 200) {
    String objetJSON = client->readString();
    String dateString = seekJSON(&objetJSON, "\"datetime\"");
    updateDateNow(dateString);
  }
  http->end();
  return true;
}


String getProductType(String* headerProductionType) {
  return seekJSON(headerProductionType, "\"production_type\"");
}

String getProductValue(String* productValueObject) {
  return seekJSON(productValueObject, "\"value\"");
}

String getStartDate(String* productValueObject) {
  return seekJSON(productValueObject, "\"start_date\"");
}

char getClearChar() {
  char c = client->read();
  if (c == (char)13) {
    client->readStringUntil((char)10);
    client->readStringUntil((char)10);
    c = client->read();
  }
  return c;
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>

#include "requestAPI.h"
#include "secret.h"
#include "productValues.h"
#include "useJSON.h"
#include "date.h"
#include "leds.h"

WiFiClientSecure* client = new WiFiClientSecure;
HTTPClient* http = new HTTPClient;
String token = "Loremipsumdolorsitametconsecteturadipiscingelit";

String getProductType(String* headerProductionType);
String getProductValue(String* productValueObject);
String getEndDate(String* productValueObject);
String getStringDatetime(String* reponseWorldTime);
char getClearChar();

void tryAndRetry(int requestIdentifier) {
  if (!WiFi.isConnected()) {
    offLeds();
    int i = 0;
    Serial.println("Start to connect");
    while (!WiFi.isConnected()) {
      Serial.print(".");
      waitStep();
      delay(100);
      i++;
    }
    addTime(i / 600);
    Serial.println(" sucess !!");
    offLeds();
  }

  for (byte j = 0; j < MAX_RETRY; j++) {
    bool sucess;
    if (requestIdentifier == REQUEST_DATE) {
      sucess = getDateTime();
    } else if (requestIdentifier == REQUEST_TOKEN) {
      sucess = getToken();
    } else if (requestIdentifier == REQUEST_MIX_ENERGETIQUE) {
      sucess = getData();
    }
    showStatement(sucess);
    if (sucess) {
      return;
    }
    delay(60000);
    addTime(1);
  }
}

bool getToken() {
  client->setInsecure();
  http->begin(*client, "https://digital.iservices.rte-france.com/token/oauth");
  http->addHeader("Content-Type", "application/x-www-form-urlencoded");
  http->addHeader("Authorization", RTE_PASS);
  int httpCode = http->POST("");
  Serial.print("======  Request RTE Token : ");
  Serial.print(httpCode);
  Serial.println("  ======");
  if (httpCode == 200) {
    String reponse = http->getString();
    reponse.remove(0, reponse.indexOf("access_token"));
    token = reponse.substring(reponse.indexOf(": \"") + 3, reponse.indexOf("\","));
  }
  http->end();
  return token.length() == 54;
}

bool getData() {
  resetListProductObjects();
  client->setInsecure();
  String header = "Bearer " + token;
  http->begin(*client, "https://digital.iservices.rte-france.com/open_api/actual_generation/v1/generation_mix_15min_time_scale?production_subtype=TOTAL&start_date=" + getStringMidnigth() + "&end_date=" + getStringForNextDay());
  http->addHeader("Authorization", header);
  int httpCode = http->GET();
  Serial.print("======  RTE Mix " + getStringMidnigth() + " -> " + getStringForNextDay() + " ( end_date: " + getStringEndDate() + " ) : ");
  Serial.print(httpCode);
  Serial.println("  ======");
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
      String endDateString;
      do {
        String productValueObject = client->readStringUntil('}').substring(1);
        endDateString = getEndDate(&productValueObject);
        if (endDateString.indexOf(getStringEndDate()) != -1) {
          productValue = getProductValue(&productValueObject);
        }
        productValueChar = getClearChar();
      } while (productValueChar == ',');

      updateValue(productionType, productValue);

      getClearChar();
      productionTypeChar = getClearChar();
    } while (productionTypeChar == ',');
  } else if (httpCode == 401) {
    http->end();
    tryAndRetry(REQUEST_TOKEN);
    return getData();
  }
  http->end();
  return dataAllGood();
}

bool getDateTime() {
  client->setInsecure();
  http->begin(*client, "https://worldtimeapi.org/api/timezone/Europe/Paris");
  int httpCode = http->GET();
  Serial.print("======  Request Datetime : ");
  Serial.print(httpCode);
  Serial.println("  ======");
  if (httpCode == 200) {
    String objetJSON = client->readString();
    String dateString = getStringDatetime(&objetJSON);
    if (dateString.length() == 34) {
      updateDateNow(dateString);
      http->end();
      return true;
    }
  }
  http->end();
  return false;
}


String getProductType(String* headerProductionType) {
  return seekJSON(headerProductionType, "\"production_type\"");
}

String getProductValue(String* productValueObject) {
  return seekJSON(productValueObject, "\"value\"");
}

String getEndDate(String* productValueObject) {
  return seekJSON(productValueObject, "\"end_date\"");
}

String getStringDatetime(String* reponseWorldTime) {
  return seekJSON(reponseWorldTime, "\"datetime\"");
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

#include "apiRTE.h"
#include "secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>

WiFiClientSecure client;
HTTPClient http;
String token;
String getProductType(String* headerProductionType);
String getProductValue(String* headerProductionType);

String stringClear(String dirtyString);
char getClearChar();
bool seekJSON(String* objetJSON, String seek, String* value);

void getToken() {
  client.setInsecure();
  http.begin(client, "https://digital.iservices.rte-france.com/token/oauth");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", RTE_PASS);
  int httpCode = http.POST("");
  if (httpCode == 200) {
    String reponse = http.getString();
    reponse.remove(0, reponse.indexOf("access_token"));
    token = reponse.substring(reponse.indexOf(": \"") + 3, reponse.indexOf("\","));
  }
  http.end();
}

bool getData() {
  String header = "Bearer " + token;
  http.begin(client, "https://digital.iservices.rte-france.com/open_api/actual_generation/v1/generation_mix_15min_time_scale?production_subtype=TOTAL&start_date=2022-08-01T00:00:00+02:00&end_date=2022-08-02T02:16:00+02:00");
  http.addHeader("Authorization", header);
  int httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode == 200) {
    client.readStringUntil('{');
    client.readStringUntil('{');
    char productionTypeChar;
    String productionType;
    do {
      String headerProductionType = client.readStringUntil('[');
      productionType = getProductType(&headerProductionType);
      char productValueChar;
      String productValue;
      do {
        String productValueObject = client.readStringUntil('}');
        productValue = getProductValue(&productValueObject);
        productValueChar = getClearChar();
      } while (productValueChar == ',');

      Serial.println(productionType);
      Serial.println(productValue);
      Serial.println();

      getClearChar();
      productionTypeChar = getClearChar();
    } while (productionTypeChar == ',');
    
    Serial.println("C'est fini !!!!");
  }
  return true;
}

String getProductType(String* headerProductionType) {
  String value;
  bool find = seekJSON(headerProductionType, "\"production_type\"", &value);
  if (find) {
    return value;
  } else {
    return "";
  }
}

String getProductValue(String* headerProductionType) {
  String value;
  bool find = seekJSON(headerProductionType, "\"value\"", &value);
  if (find) {
    return value;
  } else {
    return "";
  }
}


bool seekJSON(String* objetJSON, String seek, String* value) {
  int attrIndex;
  do {
    attrIndex = objetJSON->indexOf(',');
    String atrribut = objetJSON->substring(0, attrIndex);
    String key = stringClear(atrribut.substring(0, atrribut.indexOf(':')));
    if (key == seek) {
      *value = stringClear(atrribut.substring(atrribut.indexOf(':') + 1, atrribut.length()));
      return true;
    }
    objetJSON->remove(0, attrIndex + 1);
  } while (attrIndex != -1);
  return false;
}

String stringClear(String dirtyString) {
  if (dirtyString.indexOf((char)13) != -1) {
    int startIndex = dirtyString.indexOf((char)13);
    int lastIndex = dirtyString.lastIndexOf((char)10);
    dirtyString.remove(startIndex, lastIndex - startIndex + 1);
  }
  return dirtyString;
}

char getClearChar() {
  char c = client.read();
  if (c == (char)13) {
    client.readStringUntil((char)10);
    client.readStringUntil((char)10);
    c = client.read();
  }
  return c;
}

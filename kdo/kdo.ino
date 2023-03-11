#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define PIN 14
#define NUMPIXELS 10
#define NB_PRODUCTION_TYPE 10

const char* productionTypeList[NB_PRODUCTION_TYPE] = { "\"FOSSIL_OIL\"", "\"FOSSIL_GAS\"", "\"HYDRO\"", "\"BIOENERGY\"", "\"NUCLEAR\"", "\"FOSSIL_HARD_COAL\"", "\"WIND\"", "\"EXCHANGE\"", "\"PUMPING\"", "\"SOLAR\"" };
const int productionCO2[NB_PRODUCTION_TYPE] = { 730, 418, 6, 494, 6, 1060, 14, 420, 10, 43 };                        // geCO2/kWh
const unsigned long meanProduction[NB_PRODUCTION_TYPE] = { 688, 2356, 5046, 446, 34274, 1374, 1020, 2389, 0, 185 };  // MW
const int first_color[3] = { 0, 125, 125 };
const int last_color[3] = { 255, 0, 125 };

String token = "";
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int** color_pixels = new int*[NUMPIXELS];

void setup() {
  init_colors();
  pixels.begin();
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Start to connect");
  int coef = 1;
  int j = 0;
  while (!WiFi.isConnected()) {
    Serial.print(".");
    clear_colors();
    pixels.setPixelColor(j, pixels.Color(color_pixels[j][0], color_pixels[j][1], color_pixels[j][2]));
    pixels.show();
    j = j + coef;
    if (j + 1 == NUMPIXELS) { coef = -1; }
    if (j == 0) { coef = 1; }
    delay(100);
  }
  clear_colors();
  pixels.show();
  Serial.println("sucess");
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  getToken(&client, &http);
}

void loop() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();

  float total = 0;
  float eqCO2 = 0;
  bool sucess;
  unsigned long* productionElec = new unsigned long[NB_PRODUCTION_TYPE];

  for (int r = 0; r < NB_PRODUCTION_TYPE; r++) {
    productionElec[r] = 80000;
  }
  sucess = getData(&client, &http, productionElec);
  for (int k = 0; k < NB_PRODUCTION_TYPE; k++) {
    if (productionElec[k] >= 80000) {
      productionElec[k] = meanProduction[k];
    }
    total += productionElec[k] / 1000;
  }
  for (int k = 0; k < NB_PRODUCTION_TYPE; k++) {
    eqCO2 += productionElec[k] * productionCO2[k] / (total * 1000);
  }
  Serial.print("\nEquivalent CO2");
  Serial.print(" : ");
  Serial.println(eqCO2);

  clear_colors();
  for (int i = 0; i < NUMPIXELS; i++) {
    int d = 40;
    if (i == 0) { d = 30; }
    if (i + 1 == NUMPIXELS) { d = 50; }
    if (eqCO2 > d + 10 * i) {
      pixels.setPixelColor(i, pixels.Color(color_pixels[i][0], color_pixels[i][1], color_pixels[i][2]));
      pixels.show();
      delay(200);
    }
  }
  if (!sucess) {
    show_error();
  }
  delay(12 * 60 * 1000);
}

void getToken(WiFiClientSecure* client, HTTPClient* http) {
  http->begin(*client, "https://digital.iservices.rte-france.com/token/oauth");
  http->addHeader("Content-Type", "application/x-www-form-urlencoded");
  http->addHeader("Authorization", secret_api_rte);
  int httpCode = http->POST("");
  if (httpCode == 200) {
    StaticJsonDocument<192> tokenDoc;
    deserializeJson(tokenDoc, http->getString());
    const char* accessToken = tokenDoc["access_token"];
    token = String(accessToken);
  }
  http->end();
}

bool getData(WiFiClientSecure* stream, HTTPClient* http, unsigned long* res) {
  String key;
  String value;
  String valueString;
  bool sucess = true;

  String header = "Bearer " + token;
  http->begin(*stream, "https://digital.iservices.rte-france.com/open_api/actual_generation/v1/generation_mix_15min_time_scale?production_subtype=TOTAL");
  http->addHeader("Authorization", header);
  int httpCode = http->GET();
  Serial.println(httpCode);
  if (httpCode == 200) {
    stream->readStringUntil('{');
    stream->readStringUntil('{');
    for (int j = 0; j < NB_PRODUCTION_TYPE; j++) {
      bool error = true;
      String headerProductionType = stream->readStringUntil('[');
      int indexProduction = -1;
      for (int i = 0; i < 4; i++) {
        readJSON(&headerProductionType, &key, &value);
        Serial.print(key);
        Serial.print(" : ");
        Serial.println(value);
        if (key == "\"production_type\"") {
          indexProduction = searchIndexInProducerList(value);
        }
      }
      Serial.println();
      char c = ',';
      while (c == ',') {
        String object = stream->readStringUntil('}');
        for (int r = 0; r < 4; r++) {
          readJSON(&object, &key, &value);
          if (key == "\"value\"") {
            valueString = value;
          }
        }
        c = stream->read();
        if (c == (char)13) {
          stream->readStringUntil((char)10);
          stream->readStringUntil((char)10);
          c = stream->read();
        }
      }
      pixels.setPixelColor(0, pixels.Color(255, 0, 100));
      if (indexProduction != -1) {
        if (valueString.charAt(0) == '-') {
          res[indexProduction] = 0;
        } else {
          res[indexProduction] = atol(valueString.c_str());
        }
        if (res[indexProduction] < 80000) {
          error = false;
          pixels.setPixelColor(0, pixels.Color(0, 255, 100));
        }
      }
      pixels.show();
      delay(100);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      sucess = sucess && !error;
      stream->readStringUntil('{');
    }
    http->end();
  } else if (httpCode == 401) {
    http->end();
    Serial.println("Unauthorize get token and resend");
    getToken(stream, http);
    sucess = getData(stream, http, res);
  }
  return sucess;
}

int searchIndexInProducerList(String value) {
  for (int i = 0; i < NB_PRODUCTION_TYPE; i++)
    if (String(productionTypeList[i]) == value)
      return i;
  return -1;
}

void stringClear(String* dirtyString) {
  if (dirtyString->indexOf((char)13) != -1) {
    int startIndex = dirtyString->indexOf((char)13);
    int lastIndex = dirtyString->lastIndexOf((char)10);
    dirtyString->remove(startIndex, lastIndex - startIndex + 1);
  }
}

void readJSON(String* objetJSON, String* key, String* value) {
  int attrIndex = objetJSON->indexOf(',');
  String atrribut;
  if (attrIndex != -1) {
    atrribut = objetJSON->substring(0, attrIndex);
    objetJSON->remove(0, attrIndex + 1);
  } else {
    atrribut = *objetJSON;
  }
  *key = atrribut.substring(0, atrribut.indexOf(':'));
  *value = atrribut.substring(atrribut.indexOf(':') + 1, atrribut.length());
  stringClear(key);
  stringClear(value);
}

void init_colors() {
  for (int i = 0; i < NUMPIXELS; i++) {
    color_pixels[i] = new int[3];
    for (int j = 0; j < 3; j++) {
      color_pixels[i][j] = int(first_color[j] + (last_color[j] - first_color[j]) * i / (NUMPIXELS - 1));
    }
  }
}

void clear_colors() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
}

void show_error() {
  for (int i = 0; i < 5; i++) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
    pixels.show();
    delay(300);
    pixels.setPixelColor(0, pixels.Color(color_pixels[0][0], color_pixels[0][1], color_pixels[0][2]));
    pixels.show();
    delay(500);
  }  
}
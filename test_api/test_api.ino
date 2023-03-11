#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

String token = "";

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n =======================RESET=======================");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Start to connect");
  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("sucess");

  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  getToken(&client, &http);
  getDataStream(&client, &http);
};

void loop() {
}



void getToken(WiFiClientSecure* client, HTTPClient* http) {
  http->begin(*client, "https://digital.iservices.rte-france.com/token/oauth");
  http->addHeader("Content-Type", "application/x-www-form-urlencoded");
  http->addHeader("Authorization", secret_api_rte);
  int httpCode = http->POST("");
  if (httpCode == 200) {
    StaticJsonDocument<192> token_doc;
    deserializeJson(token_doc, http->getString());
    const char* access_token = token_doc["access_token"];
    token = String(access_token);
  }
  http->end();
  return;
}

void getDataStream(WiFiClientSecure* stream, HTTPClient* http) {
  http->begin(*stream, "https://digital.iservices.rte-france.com/open_api/actual_generation/v1/generation_mix_15min_time_scale?production_subtype=TOTAL");
  String header = "Bearer " + token;
  http->addHeader("Authorization", header);
  int httpCode = http->GET();
  Serial.println(httpCode);
  if (httpCode == 200) {
    int len = http->getSize();
    Serial.print("len: ");
    Serial.println(len);
    char buff[128];
    while (http->connected() && (len > 0 || len == -1)) {
      len = getStreamUntil(stream, "productionkjsdtype", len);
      int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));
      if (!c) {
        Serial.println("read timeout");
      }

      // write it to Serial
      Serial.write(buff, c);
      Serial.print("\n");

      if (len > 0) {
        len -= c;
      }
    }
    http->end();

  } else if (httpCode == 401) {
    http->end();
    Serial.println("Unauthorize get token and resend");
    getToken(stream, http);
    getDataStream(stream, http);
  }
  return;
}


int getStreamUntil(WiFiClientSecure* stream, String key, int len) {
  int keyLenght = key.length();
  uint8_t char_buff[1] = { 0 };
  int ind = 0;
  while ((keyLenght != ind) && (len > 0 || len == -1)) {
    stream->readBytes(char_buff, sizeof(char_buff));
    if (key[ind] == char_buff[0]) {
      ind += 1;
    } else {
      ind = 0;
    }
    len = len - 1;
    Serial.println(len);
  }
  if (len == 0) {
    Serial.println(key + " no found...");
  }
  return len;
}
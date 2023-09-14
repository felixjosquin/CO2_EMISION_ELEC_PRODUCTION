#include <Arduino.h>

#include "productValues.h"

bool isNumber(String str);

PRODUCT_OBJ listProductObjects[NB_PRODUCTION_TYPE] = {
  { .name = "\"FOSSIL_OIL\"", .productCO2 = 730, .meanProduction = 122 },
  { .name = "\"FOSSIL_GAS\"", .productCO2 = 418, .meanProduction = 4238 },
  { .name = "\"HYDRO\"", .productCO2 = 6, .meanProduction = 6920 },
  { .name = "\"BIOENERGY\"", .productCO2 = 494, .meanProduction = 1123 },
  { .name = "\"NUCLEAR\"", .productCO2 = 6, .meanProduction = 39924 },
  { .name = "\"FOSSIL_HARD_COAL\"", .productCO2 = 1060, .meanProduction = 456 },
  { .name = "\"WIND\"", .productCO2 = 14, .meanProduction = 4299 },
  { .name = "\"SOLAR\"", .productCO2 = 43, .meanProduction = 1696 },
};

void updateValue(String name, String valueString) {
  int index = -1;
  int value;
  if (!isNumber(valueString)) {
    return;
  }
  for (int i = 0; i < NB_PRODUCTION_TYPE; i++) {
    if (strcmp(listProductObjects[i].name, name.c_str()) == 0) {
      index = i;
    }
  }
  if (valueString.charAt(0) == '-') {
    value = 0;
  } else {
    value = atol(valueString.c_str());
  }
  if (index == -1 || value >= 80000) {
    return;
  }
  listProductObjects[index].value = value;
  listProductObjects[index].error = false;
}

float computeCO2() {
  int productElec = 0;
  int productCO2 = 0;
  for (int j = 0; j < NB_PRODUCTION_TYPE; j++) {
    int productValue;
    if (!listProductObjects[j].error) {
      productValue = listProductObjects[j].value;
    } else {
      productValue = listProductObjects[j].meanProduction;
    }
    productElec += productValue;
    productCO2 += listProductObjects[j].productCO2 * productValue;
  }
  Serial.println(productElec);
  Serial.println(productCO2);
  return float(productCO2) / float(productElec);
}

bool isNumber(String str) {
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if ((c < '0' || c > '9') && c != '-') {
      return false;
    }
  }
  return true;
}

void PRODUCT_OBJ::display() {
  Serial.println(this->name);
  Serial.print("  value : ");
  Serial.println(this->value);
  Serial.print("  productCO2 : ");
  Serial.println(this->productCO2);
  Serial.print("  meanProduction : ");
  Serial.println(this->meanProduction);
  Serial.print("  error : ");
  Serial.println(this->error);
  Serial.println();
}

void printListProductObjects() {
  for (int i = 0; i < NB_PRODUCTION_TYPE; i++) {
    listProductObjects[i].display();
  }
}
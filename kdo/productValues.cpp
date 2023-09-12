#include <Arduino.h>

#include "productValues.h"

bool isNumber(String str);

PRODUCT_OBJ listProductObjects[NB_PRODUCTION_TYPE] = {
  { .name = "\"FOSSIL_OIL\"", .productionCO2 = 730, .meanProduction = 688 },
  { .name = "\"FOSSIL_GAS\"", .productionCO2 = 418, .meanProduction = 2356 },
  { .name = "\"HYDRO\"", .productionCO2 = 6, .meanProduction = 5046 },
  { .name = "\"BIOENERGY\"", .productionCO2 = 494, .meanProduction = 446 },
  { .name = "\"NUCLEAR\"", .productionCO2 = 6, .meanProduction = 34274 },
  { .name = "\"FOSSIL_HARD_COAL\"", .productionCO2 = 1060, .meanProduction = 1374 },
  { .name = "\"WIND\"", .productionCO2 = 14, .meanProduction = 1020 },
  { .name = "\"PUMPING\"", .productionCO2 = 10, .meanProduction = 0 },
  { .name = "\"SOLAR\"", .productionCO2 = 43, .meanProduction = 185 },
};

void updateValue(String name, String valueString) {
  int index = -1;
  unsigned long value;
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
  Serial.print("  productionCO2 : ");
  Serial.println(this->productionCO2);
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
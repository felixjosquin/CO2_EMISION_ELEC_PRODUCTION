#include <Arduino.h>

#ifndef PRODUCT_VALUES_H
  #define PRODUCT_VALUES_H
  #define NB_PRODUCTION_TYPE 8

  typedef struct {
    const char* name;
    int value = 0;
    int productCO2;
    int meanProduction;
    bool error = true;
    void display();
  } PRODUCT_OBJ;

  void updateValue(String name,String value);
  void printListProductObjects();
  float computeCO2();

#endif

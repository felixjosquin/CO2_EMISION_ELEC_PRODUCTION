#include <Arduino.h>

#ifndef PRODUCT_VALUES_H
  #define PRODUCT_VALUES_H
  #define NB_PRODUCTION_TYPE 9

  typedef struct {
    const char* name;
    unsigned long value = 0;
    int productionCO2;
    unsigned long meanProduction;
    bool error = true;
    void display();
  } PRODUCT_OBJ;

  void updateValue(String name,String value);
  void printListProductObjects();
  
  extern PRODUCT_OBJ listProductObjects[NB_PRODUCTION_TYPE];

#endif

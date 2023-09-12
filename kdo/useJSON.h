#include <Arduino.h>

#ifndef USE_JSON_H
  #define USE_JSON_H
  
  String stringClear(String dirtyString);
  bool seekJSON(String* objetJSON, String seek, String* value);

#endif


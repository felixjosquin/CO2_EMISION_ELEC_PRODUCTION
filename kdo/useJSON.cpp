#include <Arduino.h>

#include "useJSON.h"

String seekJSON(String* objetJSON, String seek) {
  int attrIndex;
  do {
    attrIndex = objetJSON->indexOf(',');
    String atrribut = objetJSON->substring(0, attrIndex);
    String key = stringClear(atrribut.substring(0, atrribut.indexOf(':')));
    if (key == seek) {
      return stringClear(atrribut.substring(atrribut.indexOf(':') + 1, atrribut.length()));
    }
    objetJSON->remove(0, attrIndex + 1);
  } while (attrIndex != -1);
  return "";
}

String stringClear(String dirtyString) {
  while (dirtyString.indexOf((char)13) != -1) {
    // Serial.println(dirtyString);
    // for (int j = 0; j < dirtyString.length(); j++) {
    //   Serial.print((int)dirtyString[j]);
    //   Serial.print(" ");
    // }
    // Serial.println();
    int startIndex = dirtyString.indexOf((char)13);
    int lastIndex = dirtyString.indexOf((char)10, startIndex + 2) + 1;
    dirtyString.remove(startIndex, lastIndex - startIndex);
    // Serial.println(dirtyString);
    // Serial.println();
  }
  return dirtyString;
}

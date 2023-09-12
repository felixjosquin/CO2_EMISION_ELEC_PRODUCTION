#include <Arduino.h>

#include "useJSON.h"

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

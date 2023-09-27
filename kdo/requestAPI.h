#include <Arduino.h>

#ifndef REQUEST_API_H
  #define REQUEST_API_H
  #define REQUEST_DATE 1
  #define REQUEST_TOKEN 2
  #define REQUEST_MIX_ENERGETIQUE 3
  #define MAX_RETRY 3

  bool getToken();
  bool getData();
  bool getDateTime();

  void tryAndRetry(int requestIdentifier);
  
#endif

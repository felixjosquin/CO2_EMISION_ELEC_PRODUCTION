#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef LEDS_H
#define LEDS_H

#define PIN 4
  #define NUMPIXELS 10

  void initColors();
  void waitStep();
  void offLeds();
  void showCo2(float productCO2);
  void showStatement(bool sucess);
#endif

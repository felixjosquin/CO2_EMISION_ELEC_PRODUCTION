#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "leds.h"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
byte** colorPixels = new byte*[NUMPIXELS];
byte stepWait = 0;

void setColor(byte offSet, byte* color);

void waitStep() {
  clearColor();
  stepWait += 1;
  if (stepWait == 2 * NUMPIXELS) {
    stepWait = 0;
  }
  byte indice;
  if (stepWait >= NUMPIXELS) {
    indice = 2 * NUMPIXELS - stepWait - 1;

  } else {
    indice = stepWait;
  }
  setColor(indice, colorPixels[indice]);
  pixels.show();
}

void clearColor() {
  for (byte i = 0; i < NUMPIXELS; i++) {
    byte black[3] = { 0, 0, 0 };
    setColor(i, black);
  }
}

void setColor(byte offSet, byte color[3]) {
  pixels.setPixelColor(offSet, pixels.Color(color[0], color[1], color[2]));
}

void initColors() {
  pixels.begin();
  const int firstColor[3] = { 0, 125, 125 };
  const int lastColor[3] = { 255, 0, 125 };
  for (int i = 0; i < NUMPIXELS; i++) {
    colorPixels[i] = new byte[3];
    for (int j = 0; j < 3; j++) {
      colorPixels[i][j] = int(firstColor[j] + (lastColor[j] - firstColor[j]) * i / (NUMPIXELS - 1));
    }
  }
  clearColor();
  pixels.show();
}

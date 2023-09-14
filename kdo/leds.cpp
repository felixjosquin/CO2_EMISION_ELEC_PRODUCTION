#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "leds.h"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
byte** colorPixels = new byte*[NUMPIXELS];
byte stepWait = 0;

void setColor(byte offSet, byte* color);
void clearColor();

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
  offLeds();
}

void offLeds() {
  clearColor();
  pixels.show();
}

void showCo2(float productCO2) {
  const int co2RangeStep[NUMPIXELS] = { 20, 35, 40, 45, 50, 55, 65, 75, 85, 100 };
  offLeds();
  for (byte index = 0; index < NUMPIXELS; index++) {
    if (productCO2 > co2RangeStep[index]) {
      setColor(index, colorPixels[index]);
      pixels.show();
      delay(200);
    } else {
      return;
    }
  }
}

void showStatement(bool sucess) {
  byte red[3] = { 0, 255, 0 };
  byte green[3] = { 0, 255, 0 };
  byte black[3] = { 0, 0, 0 };

  if (sucess) {
    setColor(0, green);
  } else {
    setColor(0, red);
  }
  pixels.show();
  delay(100);
  setColor(0, black);
  pixels.show();
}

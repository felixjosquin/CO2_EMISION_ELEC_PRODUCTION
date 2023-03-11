#include <Adafruit_NeoPixel.h>
#define PIN 14
#define NUMPIXELS 10
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int first_color[3] = { 0, 125, 125 };
const int last_color[3] = { 255, 0, 125 };


#define pause 100


int **color_pixels = new int *[NUMPIXELS];



void setup() {
  init_colors();
  pixels.begin();
  Serial.begin(9600);
  Serial.println("\n========début========");
}
void loop() {
   

  for (int a = NUMPIXELS - 1; a >= 0; a--) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.setPixelColor(a, pixels.Color(color_pixels[a][0], color_pixels[a][1], color_pixels[a][2]));
    pixels.show();
    delay(pause);
  }
}

void init_colors() {
  for (int i = 0; i < NUMPIXELS; i++) {
    color_pixels[i] = new int[3];
    for (int j = 0; j < 3; j++) {
      color_pixels[i][j] = int(first_color[j] + (last_color[j] - first_color[j]) * i / (NUMPIXELS - 1));
    }
  }
}

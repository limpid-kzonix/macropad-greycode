#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#define LED_PIN 10
#define NUM_PIXELS 8
#define PIXEL_PIN 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Set built-in LED pin as output
  strip.begin();#include <Keyboard.h>
  strip.setBrightness(50);
  strip.show();
}

int pixelIndex = 0;

// red gradually moves from left to right
void redMove()
{
  int currentPixel = pixelIndex % NUM_PIXELS;
  int nextPixel1 = (pixelIndex + 1) % NUM_PIXELS;
  int nextPixel2 = (pixelIndex + 2) % NUM_PIXELS;
  int nextPixel3 = (pixelIndex + 3) % NUM_PIXELS;
  int nextPixel4 = (pixelIndex + 4) % NUM_PIXELS;

  strip.setPixelColor(currentPixel, strip.Color(0, 0, 0));
  strip.setPixelColor(nextPixel1, strip.Color(20, 0, 0));
  strip.setPixelColor(nextPixel2, strip.Color(80, 0, 0));
  strip.setPixelColor(nextPixel3, strip.Color(120, 0, 0));
  strip.setPixelColor(nextPixel3, strip.Color(255, 0, 0));
  strip.show();

  pixelIndex++;
}
void loop()
{
  redMove();
  delay(250);
}

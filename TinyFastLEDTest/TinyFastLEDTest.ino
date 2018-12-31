#include <FastLED.h>
#include <EEPROM.h>

#define NUM_LEDS 34
#define LED_PIN 4
#define STRIP_TYPE WS2812B
#define COLOR_MODE GRB
#define Address 0

CRGB leds[NUM_LEDS];
uint8_t hue[NUM_LEDS];
uint8_t mode = 0;
boolean repeat = true;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<STRIP_TYPE, LED_PIN, COLOR_MODE>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    hue[i] = 255 / NUM_LEDS * i;
  mode = EEPROM.read(Address);
  if (mode == 0)
    mode = 1;
  else if (mode == 1)
    mode = 2;
  else
    mode = 0;

  EEPROM.update(Address, mode);
}

void loop() {
  if (mode == 0){
    HueEffect();
  } else if (mode == 1){
    Blue();
  } else{
    HueEffect2();
  }
}

void HueEffect() {
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(hue[i]++, 255, 255);
  }

  FastLED.show();
  delay(30);
}

void HueEffect2() {
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(hue[i]++, 255, 255);
  }
  fill_solid(leds, NUM_LEDS, leds[0]);
  FastLED.show();
  delay(100);
}

void Blue() {
  if (repeat) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    repeat = false;
  }
}


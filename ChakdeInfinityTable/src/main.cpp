#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

#define LEDPIN 10
#define POT_PIN A1
#define NUM_LEDS 130

#define RAINBOW 1
#define COLOR_FUL 0

#define EEPROM_MODE 0
#define DELAY_APPLY_COLOR_ALL 100

//Functions header
void executeLEDs();
void HueEffect();
unsigned int getClr(int d);


CRGB led[NUM_LEDS];
uint8_t hue[NUM_LEDS], red = 255, green = 0, blue = 0;
byte mode = 0;
int16_t start = 0;
unsigned int clr = 0xFF0000;
bool type = false, repeat = true;
byte thm = 0;

int lastN, n;

void setup() {
    pinMode(POT_PIN, INPUT);
    FastLED.addLeds<WS2812B, LEDPIN, GRB>(led, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(250);
    FastLED.show();
    Serial.begin(9600);

    mode = EEPROM.read(EEPROM_MODE);
    if(mode == 1)
        mode = 0;
    else
        mode = 1;

    EEPROM.update(EEPROM_MODE, mode);

    for (int i = 0; i < NUM_LEDS; i++)
        hue[i] = 255 / NUM_LEDS * i;
}

void loop() {
    if(repeat)
        executeLEDs();

    int n = analogRead(POT_PIN);
    if(lastN != n){
        lastN = n;
        repeat = true;
        clr = getClr(n);
    Serial.println(n);
    Serial.println(clr);
    }
}

void executeLEDs() {
    switch(mode){
        case RAINBOW:
            HueEffect();
            break;

        case COLOR_FUL:
            repeat = false;
              for(int i=0; i< NUM_LEDS;i++)
              {
                led[i] = CRGB(red, green, blue);
                //led[i] = clr;
              }
            delay(DELAY_APPLY_COLOR_ALL);
            FastLED.show();
            break;
    }
}

unsigned int getClr(int d){
    //return (16777215/1024)*d;
    red = map(d, 0, 1024, 0, 255);
    green = map(d, 0, 1024, 255, 0);
    blue = map(d, 0, 1024, 0, 255);
}

void HueEffect()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    led[i] = CHSV(hue[i]++, 255, 255);
  }

  FastLED.show();
  delay(15);
}
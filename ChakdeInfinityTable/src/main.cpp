#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

#define LEDPIN 10
#define POT_PIN A1
#define NUM_LEDS 130

#define RAINBOW 1
#define COLOR_FUL 0
#define PERIOD 30 //For analog read window
#define EEPROM_MODE 0
#define DELAY_APPLY_COLOR_ALL 100

//Functions header
void executeLEDs();
void HueEffect();
unsigned long getClr(int d);


CRGB led[NUM_LEDS];
uint8_t hue[NUM_LEDS], red = 255, green = 0, blue = 0;
byte mode = 0;
int16_t start = 0;
unsigned long clr = 0xFF0000;
bool type = false, repeat = true;
byte thm = 0;
int lastN, n;

unsigned long colors[] = {0x00a300, 0xff0097, 0x9f00a7, 0x603cba, 0x2d89ef, 0xffc40d, 0xee1111, 0x00aba9, 0xda532c, 0xffffff};

void setup() {
    pinMode(POT_PIN, INPUT);
    FastLED.addLeds<WS2812B, LEDPIN, GRB>(led, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(250);
    FastLED.show();

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
    if(lastN - n > PERIOD || lastN - n < -PERIOD){
        lastN = n;
        repeat = true;
        clr = getClr(n/100);
    }
}

void executeLEDs() {
    switch(mode){
        case RAINBOW:
            HueEffect();
            break;

        case COLOR_FUL:
            repeat = false;
            FastLED.setBrightness(250);
            for(int i=0; i< NUM_LEDS;i++) {
                //led[i] = CRGB(red, green, blue);
                led[i] = clr;
            }
            delay(DELAY_APPLY_COLOR_ALL);
            FastLED.show();
            break;
    }
}

unsigned long getClr(int d){
    //return (16777215/1024)*d;
    if(d > 9)
        return colors[9];
    return colors[d];
}

void HueEffect() {
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CHSV(hue[i]++, 255, 255);
  }

  FastLED.show();
  delay(15);
}
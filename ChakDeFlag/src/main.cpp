#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

#define OPIN 10
#define WPIN 11
#define GPIN 12

#define NUM_LEDS 168
#define LED_GRP 5
#define C_ORANGE 0xff4500
#define C_WHITE 0xffffC8
#define C_GREEN 0x00ff00
#define Address 0

CRGB orng[NUM_LEDS], wht[NUM_LEDS], grn[NUM_LEDS];
uint8_t hue[NUM_LEDS];
uint8_t mode = 0;
int16_t start = 0;
bool type = false, onetime = true;


//Functions
void slowAirFlag(int16_t );
void slowAirFlagInit(void);

void setup() {
    FastLED.addLeds<WS2812B, OPIN, GRB>(orng, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<WS2812B, WPIN, GRB>(wht, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<WS2812B, GPIN, GRB>(grn, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(250);
    FastLED.show();
    slowAirFlagInit();
    mode = EEPROM.read(Address);
    if(mode == 1)
        mode = 0;
    else
        mode = 1;

    EEPROM.update(Address, mode);
}

void loop() {
        
    if(mode == 0)
    {
        if(start + LED_GRP >= NUM_LEDS && !type)
        {
            type = true;
            start = NUM_LEDS;
        } else if(start - LED_GRP < 0 && type)
        {
            type = false;
            start = 0;
        }
        if(type){
            start = start - LED_GRP;
        } else {
            start = start + LED_GRP;
        }

        slowAirFlag(start);
        delay(250);
    }
    else if(mode == 1 && onetime)
    {
        slowAirFlagInit();
        FastLED.show();
        onetime = false;
    }
}

void slowAirFlagInit(){
  fill_solid(orng, NUM_LEDS, C_ORANGE);
  fill_solid(wht, NUM_LEDS, C_WHITE);
  fill_solid(grn, NUM_LEDS, C_GREEN);
}

void slowAirFlag(int16_t start) {
    slowAirFlagInit();
    byte cof = LED_GRP / 150;

    for(int i=0; i<LED_GRP; i++){
        //Green
        grn[start + i].g = 50 + cof;

        //White
        wht[start + i].r = 50 + cof;
        wht[start + i].g = 50 + cof;
        wht[start + i].b = 50 + cof;

        //Orange
        orng[start + i].r = 100;
        orng[start + i].g = 50 + cof;
    }

    FastLED.setBrightness(250);
    FastLED.show();
}

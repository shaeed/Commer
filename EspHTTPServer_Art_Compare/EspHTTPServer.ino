//#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <EEPROM.h>

//LED 
#define NUM_LEDS 890
#define PIN 4
#define LED_PIN 4
#define STRIP_TYPE WS2812B
#define COLOR_MODE GRB
CRGB leds[NUM_LEDS];
uint8_t hue[NUM_LEDS];

//Wifi
#define debug true
const char* ssid = "dimensionless art studio";
const char* password = "Dimen@123";

// Start a TCP Server on port 5045
//WiFiServer server(5045);
ESP8266WebServer server ( 5045 );
#define DEV_NAME  "InfinityRoom"
//WiFiClient client;

//Setup constants
#define THEME_OFF         1 
#define THEME_ON          2 
#define THEME_RAINBOW     3 
#define THEME_STROBE      4 
#define THEME_ALLWHITE    5 
#define THEME_COLORFUL    6 
#define THEME_FADE        7 
#define THEME_NIGHTSKY    10
#define Theme_RGBLoop                 11
#define Theme_FadeInFadeOut           12
#define Theme_Strobe2                 13
#define Theme_CylonBounce             14
#define Theme_NewKITT                 15
#define THEME_NIGHTSKY2               16
#define Theme_Sparkle                 17
#define Theme_SnowSparkle             18
#define Theme_RunningLights           19
#define Theme_colorWipe               20
#define Theme_theaterChaseRainbow     21
#define Theme_Fire                    22
#define Theme_meteorRain              23
#define Theme_HueEffect               24

#define THEME_CUSTOM      100

#define DELAY_APPLY_COLOR_ALL 100
#define DELAY_MICRO 20

//Globals
boolean dataAvailable = false;
String data;

byte theme;
byte themeBrt;
byte themeBrtFade;
uint16_t themeSpd;
unsigned long themeClr;
boolean isRepeat;
boolean isApplyColor, isApplyBrt;
int hueInt;
unsigned long counter;
boolean showBlack;

byte tThm, tBrt, tApply = 0;
boolean tApplyClr, tApplyBrt, fillLed;
uint16_t tSpd, tLed;
unsigned long tClr;
byte r, g, b;

void processData(){
  tApply = 11;
  if(tApplyBrt){
    FastLED.setBrightness(themeBrt);
    dataAvailable = false;
  }
}

void handleApply() {
  dataAvailable = true;
  
  if (server.arg("apply")!= "" && server.arg("apply") == "True"){
    tApply = 1;
  }
  if (server.arg("brt")!= ""){
    tBrt = server.arg("brt").toInt();
    tApplyBrt = true;
  }
  if (server.arg("clr")!= ""){
    tClr = server.arg("clr").toInt();
    tApplyClr = true;
  }
  if (server.arg("led")!= ""){
    tLed = server.arg("clr").toInt();
    fillLed = true;
  }
  if (server.arg("spd")!= ""){
    tSpd = server.arg("spd").toInt();
  }
  if (server.arg("thm")!= ""){
    tThm = server.arg("thm").toInt();
  }

  Serial.println(String(tThm) + " " + String(tBrt));
  //tring s = server.readStringUntil('\r');
  //Serial.println(s);
  displayPage("Hndle Apply");
}

void applyLed(){
  tApply = false;
  themeBrt = tBrt;
  themeSpd = tSpd;
  themeClr = tClr;
  theme = tThm;
  isRepeat = true;
  isApplyColor = tApplyClr;
  isApplyBrt = tApplyBrt;
  
  counter = 0;
  hueInt = 0;
  themeBrtFade = themeBrt;

  //Convert to RGB
  b = tClr & 255;
  g = (tClr >> 8) & 255;
  r = (tClr >> 16) & 255;
}

void executeLED(){
  if(isApplyBrt){
    isApplyBrt = false;
    FastLED.setBrightness(themeBrt);
  }
  switch(theme){
    case THEME_OFF:
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        isRepeat = false;
        delay(DELAY_APPLY_COLOR_ALL);
        FastLED.show();
        break;
        
    case THEME_RAINBOW:
         fill_rainbow(leds, NUM_LEDS, hueInt, 5);
         FastLED.show();
         hueInt = hueInt + themeSpd;
        break;
        
    case THEME_STROBE:
        if(counter >= 1000 * themeSpd){
          counter = 0;
          if(showBlack){
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            delay(DELAY_APPLY_COLOR_ALL);
            FastLED.show();
            //delay(600);
            showBlack = false;
          } else {
            /*for(int i=0; i< NUM_LEDS; i++) {
              leds[i] = CRGB(255, 0, 0);
            }*/
            fill_solid(leds, NUM_LEDS, themeClr);
            delay(DELAY_APPLY_COLOR_ALL);
            FastLED.show();
            showBlack = true;
          }
        }
        counter++;
        break;
        
    case THEME_ALLWHITE:
        fill_solid(leds, NUM_LEDS, CRGB::White);
        delay(DELAY_APPLY_COLOR_ALL);
        FastLED.show();
        isRepeat = false;
        break;
        
    case THEME_COLORFUL:
        fill_solid(leds, NUM_LEDS, themeClr);
        delay(DELAY_APPLY_COLOR_ALL);
        FastLED.show();
        isRepeat = false;
        break;
        
    case THEME_FADE:
        if(isApplyColor){
          fill_solid(leds, NUM_LEDS, themeClr);
          delay(DELAY_APPLY_COLOR_ALL);
          isApplyColor = false;
        }
        if(counter >= 1000 * themeSpd){
          counter = 0;
          if(showBlack){
            //Fade in
            FastLED.setBrightness(themeBrtFade);
            themeBrtFade++;
            delay(2);
            FastLED.show();
            if(themeBrtFade > themeBrtFade)
              showBlack = false;
          } else {
            //Fade out
            FastLED.setBrightness(themeBrtFade);
            themeBrtFade--;
            delay(2);
            FastLED.show();
            if(themeBrtFade < 1)
              showBlack = true;
          }
        }
        counter++;
        break;

       case Theme_RGBLoop:
      RGBLoop();
   break;
   
   case Theme_FadeInFadeOut:
      FadeInOut(r, g, b);
   break;
   
   case Theme_Strobe2:
      Strobe(r, g, b, 10, themeSpd, 1000);
   break;
   
   case Theme_CylonBounce:
      CylonBounce(r, g, b, 4, themeSpd, 50);
   break;
   
   case Theme_NewKITT:
      NewKITT(r, g, b, 20, themeSpd, 50);
   break;
   
   case THEME_NIGHTSKY:
      Twinkle(0xff, 0xff, 0xff, 100, themeSpd, false);
      //Twinkle(0, 0, 0, 500, themeSpd, false);
   break;
   
   case THEME_NIGHTSKY2:
      TwinkleRandom(200, themeSpd, false);
   break;
   
   case Theme_Sparkle:
      Sparkle(r, g, b, themeSpd);
   break;
   
   case Theme_SnowSparkle:
      SnowSparkle(r, g, b, themeSpd, random(100, 1000));
   break;
   
   case Theme_RunningLights:
      RunningLights(r, g, b, themeSpd);
   break;
   
   case Theme_colorWipe:
      colorWipe(r, g, b, themeSpd);
   break;
   
   case Theme_theaterChaseRainbow:
      theaterChaseRainbow(50);
   break;
   
   case Theme_Fire:
      Fire(35, 180, themeSpd);
   break;
   
   case Theme_meteorRain:
      meteorRain(r, g, b, 100, 64, true, themeSpd);
   break;
   
   case Theme_HueEffect:
     
     HueEffect();
   break;
        
    case THEME_CUSTOM:
        FastLED.show();
        break;
  }
}

void fillLedData(){
  fillLed = false;
  leds[tLed] = tClr;
  
}

void setup() {
  //Init Hardware
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<STRIP_TYPE, LED_PIN, COLOR_MODE>(leds, NUM_LEDS);
  if(debug){
    Serial.begin(115200);
    //Serial.begin(1843200);
    delay(100);
  }
  
  //Wifi connect
  //WiFiManager wifiManager;
  //wifiManager.autoConnect("InfinityRoom");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(debug)
    Serial.print(".");
  }
  if(debug){
    Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  }
  
  //Reset automatically
  boolean isReset;
  int eeAddress = 0;
  EEPROM.get(eeAddress, isReset);
  if(isReset){
    isReset = false;
    EEPROM.put(eeAddress, isReset);
    ESP.restart();
  } else {
    isReset = true;
    EEPROM.put(eeAddress, isReset);
  }

  //Page handrels
  server.on ( "/", handleRoot );
  server.on ("/ir", handleApply); //Infinity room
  server.on ("/irl", handleLeds);
  
  // Start the server
  server.begin();
  Serial.println("Server started"); 
  //client = server.available(); 

  for (int i = 0; i < NUM_LEDS; i++)
    hue[i] = 255 / NUM_LEDS * i;
  FastLED.show();

}

void loop() {
  //TCPServer();
  server.handleClient();
  
  if(dataAvailable)
    processData();
    
  if(tApply > 10)
    applyLed();

  if(fillLed)
    fillLedData();
    
  if(isRepeat)
    executeLED();
}

void handleRoot() {
  displayPage(DEV_NAME);
}

void handleLeds(){

  displayPage("Handle LEDs");
}

void displayPage(String page){
  //Send the page to client
  server.send ( 200, "text/html", page );
}

//If returns True, then break the current task
boolean setDelay(uint16_t d){
  unsigned long mili = d * 1000;
  for(unsigned long i = 0; i < mili; i += DELAY_MICRO){
    server.handleClient();
    if(dataAvailable){
      //Some data received from user
      //Break the timing
      processData();
      if(dataAvailable)
        return true;
    } else {
      delayMicroseconds(DELAY_MICRO);
    }
  }
  return false;
}


void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}

void HueEffect()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue[i]++, 255, 255);
  }

  FastLED.show();
  //delay(15);
  if(setDelay(15))
    return;
}

void RGBLoop() {
  for (int j = 0; j < 3; j++ ) {
    // Fade IN
    for (int k = 0; k < 256; k++) {
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      FastLED.show();
      if(setDelay(15))
       return;
    }
    // Fade OUT
    for (int k = 255; k >= 0; k--) {
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      FastLED.show();
      if(setDelay(15))
       return;
    }
  }
}

void FadeInOut(byte red, byte green, byte blue) {
  float r, g, b;

  for (int k = 0; k < 256; k = k + 1) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    FastLED.show();
  }

  for (int k = 255; k >= 0; k = k - 2) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    FastLED.show();
  }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    setAll(red, green, blue);
    FastLED.show();
    if(setDelay(FlashDelay)) return;
    setAll(0, 0, 0);
    FastLED.show();
    if(setDelay(FlashDelay)) return;
  }

  if(setDelay(EndPause)) return;
}

void HalloweenEyes(byte red, byte green, byte blue,
                   int EyeWidth, int EyeSpace,
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause) {
  randomSeed(analogRead(0));

  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2 * EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;

  for (i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }

  FastLED.show();

  if (Fade == true) {
    float r, g, b;

    for (int j = Steps; j >= 0; j--) {
      r = j * (red / Steps);
      g = j * (green / Steps);
      b = j * (blue / Steps);

      for (i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }

      FastLED.show();
      if(setDelay(FadeDelay)) return;
    }
  }

  setAll(0, 0, 0); // Set all black

  if(setDelay(EndPause)) return;
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {

  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }

  if(setDelay(ReturnDelay)) return;

  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }

  if(setDelay(ReturnDelay)) return;
}

void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= 0; i--) {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
  if(setDelay(ReturnDelay)) return;
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i <= ((NUM_LEDS - EyeSize) / 2); i++) {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
  if(setDelay(ReturnDelay)) return;
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
  if(setDelay(ReturnDelay)) return;
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--) {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
  if(setDelay(ReturnDelay)) return;
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++) {
    setPixel(random(NUM_LEDS), red, green, blue);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }

  if(setDelay(SpeedDelay)) return;
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++) {
    setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }

  if(setDelay(SpeedDelay)) return;
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  if(setDelay(SpeedDelay)) return;
  setPixel(Pixel, 0, 0, 0);
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red, green, blue);

  int Pixel = random(NUM_LEDS);
  setPixel(Pixel, 0xff, 0xff, 0xff);
  FastLED.show();
  if(setDelay(SparkleDelay)) return;
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  if(setDelay(SpeedDelay)) return;
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int i = 0; i < NUM_LEDS * 2; i++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUM_LEDS; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    FastLED.show();
    if(setDelay(WaveDelay)) return;
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        setPixel(i + q, red, green, blue);  //turn every third pixel on
      }
      FastLED.show();

      if(setDelay(SpeedDelay)) return;

      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        setPixel(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(int SpeedDelay) {
  byte *c;

  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        c = Wheel( (i + j) % 255);
        setPixel(i + q, *c, *(c + 1), *(c + 2)); //turn every third pixel on
      }
      FastLED.show();

      if(setDelay(SpeedDelay)) return;

      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        setPixel(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  FastLED.show();
  if(setDelay(SpeedDelay)) return;
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}


void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    FastLED.show();
    if(setDelay(SpeedDelay)) return;
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue ); 
}


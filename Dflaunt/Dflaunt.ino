#include <WiFiManager.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

//LED 
#define NUM_LEDS 180
#define LED_PIN 4
#define STRIP_TYPE WS2812B
#define COLOR_MODE GRB
CRGB leds[NUM_LEDS];

//Wifi
//const char* ssid = "GreenRose";
//const char* password = "shaeedkhan";
ESP8266WebServer server ( 80 );

//Setup constants
#define THEME_OFF         0 
#define THEME_ON          80 //Not used, so any large value 
#define THEME_RAINBOW     1
#define THEME_STROBE      2 
#define THEME_ALLWHITE    3 
#define THEME_COLORFUL    4 
#define THEME_FADE        5 
#define THEME_SPIRAL      6 
#define THEME_BLINK       7 
#define THEME_NIGHTSKY    8

#define DELAY_APPLY_COLOR_ALL 100

#define LED1 20
#define LED2 21
#define LED3 22
#define LED4 23

//Global Variables
byte theme;
byte themeBrt;
byte themeBrtFade;
uint16_t themeSpd;
unsigned long themeClr;
boolean isRepeat;
boolean isApplyColor;
int hue;
unsigned long counter;
boolean showBlack;

#define N3W_LED 4
byte ledDelay[N3W_LED];
boolean ledStatus[N3W_LED];


void setup() {
  //Init Hardware
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<STRIP_TYPE, LED_PIN, COLOR_MODE>(leds, NUM_LEDS);
  delay(100);

  //Wifi connect
  WiFiManager wifiManager;
  wifiManager.autoConnect("Dflaunt");
  //Initial page
  server.on ( "/", handleRoot );
  server.on ("/apply", handleApply);
  server.on ("/ledswitch", handleLeds);

  // Start the server
  server.begin();
  
  //Initial
  theme = 1;
  themeBrt = 10;
  themeSpd = 1;
}

void loop() {
  server.handleClient();
  if(isRepeat)
    executeLED();
}

void handleRoot() {
  displayPage();
}

void handleApply() {
  //Serial.println("rec");
  isRepeat = true;
  isApplyColor = true;
  
  if (server.arg("thm")!= ""){
    theme = server.arg("thm").toInt();
  }
  if (server.arg("brt")!= ""){
    themeBrt = server.arg("brt").toInt();
  }
  if (server.arg("spd")!= ""){
    themeSpd = server.arg("spd").toInt();
  }
  if (server.arg("color")!= ""){
    themeClr = server.arg("color").toInt();
  }

  displayPage();
  counter = 0;
  hue = 0;
  themeBrtFade = themeBrt;
}

void executeLED(){
  FastLED.setBrightness(24*themeBrt);
  switch(theme){
    case THEME_OFF:
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        isRepeat = false;
        delay(DELAY_APPLY_COLOR_ALL);
        FastLED.show();
        break;
        
    case THEME_ON:
        
        break;
        
    case THEME_RAINBOW:
         fill_rainbow(leds, NUM_LEDS, hue, 15);
         FastLED.show();
         hue = hue + themeSpd;
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
        /*for(int i=0; i< NUM_LEDS; i++)
        {  
          leds[i] = CRGB(255,255,255);
        }*/
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
            FastLED.setBrightness(24*themeBrtFade);
            themeBrtFade++;
            delay(2);
            FastLED.show();
            if(themeBrtFade > themeBrtFade)
              showBlack = false;
          } else {
            //Fade out
            FastLED.setBrightness(24*themeBrtFade);
            themeBrtFade--;
            delay(2);
            FastLED.show();
            if(themeBrtFade < 1)
              showBlack = true;
          }
        }
        counter++;
        break;
        
    case THEME_SPIRAL:
        
        break;
        
    case THEME_BLINK:
        
        break;
        
    case THEME_NIGHTSKY:
        
        break;
  }
}

void handleLeds(){
  byte led;
  String stat;
  if (server.arg("led")!= ""){
    led = server.arg("led").toInt() - 1;
  } else {
    led = 0;
  }
  if (server.arg("stat")!= ""){
    stat = server.arg("stat");
  }
  if(stat == "ON"){
    ledStatus[led] = true;
  } else {
    ledStatus[led] = false;
  }
  if (server.arg("brt")!= ""){
    ledDelay[led] = server.arg("brt").toInt();
  } else {
    ledDelay[led] = 1;
  }
  displayPage();
  change3WLed(led);
}

void change3WLed(byte led){
  if(ledStatus[led]){
    //Send to UART on uno
  } else {
    //Send to UART on uno
  }
}

void displayPage(){
  String page = "";
page += "<!DOCTYPE html><html><head><meta charset='ISO-8859-1'>";
page += "<title>Eclairage - D-FLAUNT Control Center</title>";
page += "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.12.1/jquery-ui.css\">";
page += "<script type=\"text/javascript\" src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>";
page += "<script type=\"text/javascript\" src=\"https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.12.1/jquery-ui.js\"></script>";
page += "<style>html * {font-size: 25px; color: #FFFFFF; font-family: Arial;  text-align: center;}";
page += "button, input, select,option {color: #000000;} table {border-spacing: 5px 1rem; height: 80%;}</style>";
page += "<script type='text/javascript'>";
page += "$( function() { populateThms();";
page += "populateColors($('#color_selected').val());";
page += "$( '.thm-rd' ).checkboxradio({icon: false});";
page += "$('#'+$('#thm_selected').val()).prop('checked', true).button('refresh');";
page += "});";

page += "function selectRadio() {";
page += "var t = document.getElementById('thm_selected').value;";
page += "document.getElementById(t).checked = true; populateColors(document.getElementById('color_selected').value);}";
page += "";
page += "function populateColors(selectedColor){";
page += "var clrs = ['AliceBlue', 'Amethyst', 'AntiqueWhite', 'Aqua', 'Aquamarine', 'Azure', 'Beige', 'Bisque', 'Black', 'BlanchedAlmond', 'Blue', 'BlueViolet', 'Brown', 'BurlyWood', 'CadetBlue', 'Chartreuse', 'Chocolate', 'Coral', 'CornflowerBlue', 'Cornsilk', 'Crimson', 'Cyan', 'DarkBlue', 'DarkCyan', 'DarkGoldenrod', 'DarkGray', 'DarkGreen', 'DarkKhaki', 'DarkMagenta', 'DarkOliveGreen', 'DarkOrange', 'DarkOrchid', 'DarkRed', 'DarkSalmon', 'DarkSeaGreen', 'DarkSlateBlue', 'DarkSlateGray', 'DarkTurquoise', 'DarkViolet', 'DeepPink', 'DeepSkyBlue', 'DimGray', 'DodgerBlue', 'FireBrick', 'FloralWhite', 'ForestGreen', 'Fuchsia', 'Gainsboro', 'GhostWhite', 'Gold', 'Goldenrod', 'Gray', 'Green', 'GreenYellow', 'Honeydew', 'HotPink', 'IndianRed', 'Indigo', 'Ivory', 'Khaki', 'Lavender', 'LavenderBlush', 'LawnGreen', 'LemonChiffon', 'LightBlue', 'LightCoral', 'LightCyan', 'LightGoldenrodYellow', 'LightGreen', 'LightGrey', 'LightPink', 'LightSalmon', 'LightSeaGreen', 'LightSkyBlue', 'LightSlateGray', 'LightSteelBlue', 'LightYellow', 'Lime', 'LimeGreen', 'Linen', 'Magenta', 'Maroon', 'MediumAquamarine', 'MediumBlue', 'MediumOrchid', 'MediumPurple', 'MediumSeaGreen', 'MediumSlateBlue', 'MediumSpringGreen', 'MediumTurquoise', 'MediumVioletRed', 'MidnightBlue', 'MintCream', 'MistyRose', 'Moccasin', 'NavajoWhite', 'Navy', 'OldLace', 'Olive', 'OliveDrab', 'Orange', 'OrangeRed', 'Orchid', 'PaleGoldenrod', 'PaleGreen', 'PaleTurquoise', 'PaleVioletRed', 'PapayaWhip', 'PeachPuff', 'Peru', 'Pink', 'Plaid', 'Plum', 'PowderBlue', 'Purple', 'Red', 'RosyBrown', 'RoyalBlue', 'SaddleBrown', 'Salmon', 'SandyBrown', 'SeaGreen', 'Seashell', 'Sienna', 'Silver', 'SkyBlue', 'SlateBlue', 'SlateGray', 'Snow', 'SpringGreen', 'SteelBlue', 'Tan', 'Teal', 'Thistle', 'Tomato', 'Turquoise', 'Violet', 'Wheat', 'White', 'WhiteSmoke', 'Yellow', 'YellowGreen'];";
page += "var vals = ['15792383', '10053324', '16444375', '65535', '8388564', '15794175', '16119260', '16770244', '0', '16772045', '255', '9055202', '10824234', '14596231', '6266528', '8388352', '13789470', '16744272', '6591981', '16775388', '14423100', '65535', '139', '35723', '12092939', '11119017', '25600', '12433259', '9109643', '5597999', '16747520', '10040012', '9109504', '15308410', '9419919', '4734347', '3100495', '52945', '9699539', '16716947', '49151', '6908265', '2003199', '11674146', '16775920', '2263842', '16711935', '14474460', '16316671', '16766720', '14329120', '8421504', '32768', '11403055', '15794160', '16738740', '13458524', '4915330', '16777200', '15787660', '15132410', '16773365', '8190976', '16775885', '11393254', '15761536', '14745599', '16448210', '9498256', '13882323', '16758465', '16752762', '2142890', '8900346', '7833753', '11584734', '16777184', '65280', '3329330', '16445670', '16711935', '8388608', '6737322', '205', '12211667', '9662683', '3978097', '8087790', '64154', '4772300', '13047173', '1644912', '16121850', '16770273', '16770229', '16768685', '128', '16643558', '8421376', '7048739', '16753920', '16729344', '14315734', '15657130', '10025880', '11529966', '14381203', '16773077', '16767673', '13468991', '16761035', '13391155', '14524637', '11591910', '8388736', '16711680', '12357519', '267920', '9127187', '16416882', '16032864', '3050327', '16774638', '10506797', '12632256', '8900331', '6970061', '7372944', '16775930', '65407', '4620980', '13808780', '32896', '14204888', '16737095', '4251856', '15631086', '16113331', '16777215', '16119285', '16776960', '10145074'];";
page += "var drp = document.getElementById('color');";
page += "for(var i=0;i<clrs.length;i++){";
page += "var opt = document.createElement('option');";
page += "opt.innerHTML = clrs[i];";
page += "opt.value = vals[i];";
page += "if(vals[i]==selectedColor) opt.selected = 'selected';";
page += "drp.appendChild(opt);";
page += "}}";
page += "function populateThms(){";
page += "var thms = ['Off', 'Rainbow', 'Strobe', 'All White', 'Colorful ', 'Fade In/Out', 'Spiral', 'Blink', 'Night Sky'];";
page += "for(var i=0;i<thms.length;i++){";
page += "var content = '<label for=\"' + i + '\">' + thms[i] + '</label>';";
page += "content += \"<input type='radio' name='thm' class='thm-rd' value='\"+ i +\"' id='\" + i + \"'>\";";
page += "$('#thm-div').append(content);";
page += "}}";
page += "</script>";

page += "</head><body bgcolor='#481036'><h1>D-FLAUNT Control Center</h1><hr>";
page += "<form action='/apply'>";
page += "<div id='thm-div'></div>";
page += "<table>";
page += "<tr><td>Color:</td><td><select name='color' id='color'><option></option></select></td></tr>";
page += "<tr><td>Brightness:</td><td><input type='number' name='brt' min='1' max='10' value='"+String(themeBrt)+"'></td></tr>";
page += "<tr><td>Speed/Hue:</td><td><input type='number' name='spd' min='1' value='"+String(themeSpd)+"' max='10000'></td></tr>";
page += "<tr><td colspan='2'><input type='submit' value='Apply'></td></tr>";
page += "</table></form>";

page += "<input type='text' value='" + String(theme) + "' id='thm_selected' hidden>";
page += "<input type='text' value='"+ String(themeClr) +"' id='color_selected' hidden>";
page += "</body></html>";

  //Send the page to client
  server.send ( 200, "text/html", page );
}

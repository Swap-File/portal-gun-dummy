#include "FastLED.h"
#include <math.h>

FASTLED_USING_NAMESPACE

#define NUM_LEDS    40
CRGB leds[NUM_LEDS];

uint8_t gloveid;

void setup() {

  FastLED.addLeds<LPD8806, 9, 3, RBG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);

  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  pinMode(5, OUTPUT); //orange
  pinMode(6, OUTPUT); //blue

  pinMode(10, INPUT_PULLUP);  //glovemode
  if (digitalRead(10))   gloveid = 0;
  else                   gloveid = 1;

}

int global_mode = 0;
bool button_blue_previous = false;
bool button_orange_previous = false;
int spin = 0;

void loop()
{

  bool button_blue = false;
  bool button_orange = false;

  if (gloveid) {
    if (digitalRead(7) == false) button_blue = true;
    if (digitalRead(4)  == false) button_orange = true;
  }
  else {
    if (digitalRead(4) == false) button_blue = true;
    if (digitalRead(7) == false) button_orange = true;
  }

  if ( button_blue_previous !=  button_blue && button_blue == true) {
    if (global_mode > 0) global_mode = 0;
    else    global_mode = -1;
  }
  if ( button_orange_previous !=  button_orange && button_orange == true)  {
    if (global_mode < 0) global_mode = 0;
    else    global_mode = 1;
  }
  
  if (gloveid) {
    if (global_mode > 0) analogWrite(5, 255);
    else analogWrite(5, 0);

    if (global_mode < 0) analogWrite(6, 255);
    else analogWrite(6, 0);
  } else {
    if (global_mode > 0) analogWrite(6, 255);
    else analogWrite(6, 0);

    if (global_mode < 0) analogWrite(5, 255);
    else analogWrite(5, 0);
  }
  
  CRGB color;
  if (global_mode > 0)          color = CRGB(0, 30, 224);
  else if (global_mode < 0)     color = CRGB(214, 40, 0);
  else                          color = CRGB(0, 0, 0);

  float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
  CRGB faded_color = color;
  faded_color.fadeToBlackBy(val);

  for (int i = 0; i < 40; i++) {
    if (i == spin || i == ( spin + 20)) leds[i] = color;
    else                                leds[i] = faded_color;
  }

  FastLED.show();

  EVERY_N_MILLISECONDS( 20 ) spin++;
  if (spin >= 20) spin = 0;

  button_blue_previous =  button_blue;
  button_orange_previous = button_orange;
}





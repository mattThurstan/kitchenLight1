/*
    'kitchenLight1_A' by Thurstan. PIR triggered LED strips.
    Copyright (C) 2020 MTS Standish (Thurstan|mattKsp)
    
    A does one side of the kitchen and B does the other side.
    Each side has 2 LED strips above the units and lights below the units, operated seperately. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be usefull,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    https://github.com/mattThurstan/
*/

//Board = WEMOS D1 Mini with ESP8266

/*----------------------------libraries----------------------------*/
#include <MT_LightControlDefines.h>
#include <EEPROM.h>                           // a few saved settings
#include <painlessMesh.h>                     // https://github.com/gmag11/painlessMesh
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include <NeoPixelBrightnessBus.h>            // NeoPixelBrightnessBus (just for ESP8266)- for brightness functions (instead of NeoPixelBus.h)

/*----------------------------system----------------------------*/
const String _progName = "kitchenLight1_A";
const String _progVers = "0.1";               //init

boolean DEBUG_GEN = false;                    // realtime serial debugging output - general
boolean DEBUG_OVERLAY = false;                // show debug overlay on leds (eg. show segment endpoints, center, etc.)
boolean DEBUG_MESHSYNC = false;               // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
boolean DEBUG_COMMS = false;                  // realtime serial debugging output - comms
boolean DEBUG_USERINPUT = false;              // realtime serial debugging output - user input

boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              // this should init false, then get activated by input - on/off true/false
bool shouldSaveSettings = false;              // flag for saving data
bool runonce = true;                          // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;               // just in case  - using FastLED.delay instead..

bool _isBreathing = false;                    // toggle for breath
bool _isBreathOverlaid = false;               // toggle for whether breath is overlaid on top of modes
bool _isBreathingSynced = false;              // breath sync local or global

/*----------------------------D1 mini pins----------------------------*/
const byte _ledDOut0Pin = 6;                  //DOut 0 -> LED strip 0 DIn
const byte _ledDOut1Pin = 7;                  //DOut 1 -> LED strip 1 DIn
const byte _pirPin = 10;                      //PIR sensor pin
MT_BlinkStatusLED statusLED(13);              //setup status LED (internal) on pin 13

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 1;
//int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
int _modePreset[_modePresetSlotNum] = { 4 };
volatile int _modeCur = 4;                    //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                   //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*----------------------------buttons----------------------------*/

/*----------------------------sensors----------------------------*/
unsigned long _pirHoldTime = 10000;           //time (in millis) that the lights are on for
long _pirPrevMillis = 0;                      //used to save the time at the point the PIR is triggered
boolean _pirToggled = false;                  //used for hold loop
const unsigned long _loopPirInterval = 50;    //read loop interval in milliseconds for PIR sensor

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNum = 100;                      //total number of LEDs
const int _segmentTotal = 2;                  //total segments in use (in this case each segment is 1 strip)
const int _ledGlobalBrightness = 255;         //global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;            //current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;          //the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                //main loop FastLED show delay //100
LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 49, 50 }, 
  { 50,99, 50 }
};
CRGB leds[_ledNum];                           //global RGB array
                                              
//int _ledState = LOW;                          //use to toggle LOW/HIGH (ledState = !ledState)
//#define TEMPERATURE_0 WarmFluorescent
//#define TEMPERATURE_1 StandardFluorescent
//#define TEMPERATURE_2 CoolWhiteFluorescent
//int _colorTempCur = 1;                        //current colour temperature       ???

/*----------------------------Mesh----------------------------*/


/*----------------------------MAIN----------------------------*/
void setup() {
  
  Serial.begin(9600);                         // ???
  //Serial.begin(115200);
  
  #ifdef DEBUG
    Serial.println();
    Serial.print(_progName);
    Serial.print(" v.");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  #endif
  
  setupPir();                                 //setup the PIR sensor
  delay(3000);                                //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();                                //setup the LEDs
  setupUserInputs();                          //setup any direct user input eg. buttons

  #ifdef DEBUG
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println();
  #endif
  statusLED.Blink3();
}

void loop() {
  
//  if(_firstTimeSetupDone == false) {
//    #ifdef DEBUG
//    //
//    #endif
//    _firstTimeSetupDone = true;               //need this for stuff like setting sunrise, cos it needs the time to have been set
//  }

  //comms();                                    //loop communications
  loopPir();                                  //loop PIR sensor
  loopUserInputs();                           //loop direct user input
  loopModes();                                //loop modes

  FastLED.show();                             //send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}

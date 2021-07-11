/*
    'kitchenLight1_Mesh' by Thurstan. PIR triggered LED strips.
    Copyright (C) 2021 MTS Standish (Thurstan|mattKsp)
    
    A does one side of the kitchen and B does the other side.
    Each side has LED strips above the units and lights below the units, operated seperately. 

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

// Wemos D32 Pro (ESP32-WROVER)

/*----------------------------libraries------------------------*/
#include <MT_LightControlDefines.h>
#include <Preferences.h>                      // a few saved settings
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version
#include <painlessMesh.h>                     // https://github.com/gmag11/painlessMesh

/*----------------------------system---------------------------*/
const String _progName = "kitchenLight1_Mesh";
const String _progVers = "0.206";             // memory

uint8_t LOCKDOWN_SEVERITY = 0;                // the severity of the lockdown
bool LOCKDOWN = false;                        // are we in lockdown?

bool DEBUG_GEN = false;                       // realtime serial debugging output - general
bool DEBUG_OVERLAY = false;                   // show debug overlay on leds (eg. show segment endpoints, center, etc.)
bool DEBUG_MESHSYNC = false;                  // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
bool DEBUG_COMMS = false;                     // realtime serial debugging output - comms
bool DEBUG_INTERRUPT = false;                 // realtime serial debugging output - interrupt pins
bool DEBUG_USERINPUT = true;                  // realtime serial debugging output - user input

bool _firstTimeSetupDone = false;             // starts false
bool _dayMode = false;                        // whether or not to run if night or day. default to night just so it works in case something goes wrong.
bool _shouldSaveSettings = false;             // flag for saving data
bool _runonce = true;                         // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;                 // just in case  - using FastLED.delay instead..

// top lights
//volatile boolean _topOnOff = false;           // top lights on/off true/false - this should init false, then get activated by input
volatile bool _topOnOff = false;
bool _topIsBreathing = false;                 // toggle for breath
bool _topIsBreathOverlaid = false;            // toggle for whether breath is overlaid on top of modes
bool _topIsBreathingSynced = false;           // breath sync local or global
// bottom lights
//volatile boolean _botOnOff = false;           // bottom lights on/off true/false - this should init false, then get activated by input
volatile bool _botOnOff = false;
bool _botIsBreathing = false;                 // toggle for breath
bool _botIsBreathOverlaid = false;            // toggle for whether breath is overlaid on top of modes
bool _botIsBreathingSynced = false;           // breath sync local or global

Preferences preferences;

/*----------------------------pins----------------------------*/
//5v regulated power is connected to USB (VBUS)
const int _ledDOut0Pin = 0;                   // DOut 0 -> LED strip 0 DIn   - top right
const int _ledDOut1Pin = 4;                   // DOut 1 -> LED strip 1 DIn   - top left
//const int _ledDOut2Pin = 2;                   // DOut 2 -> LED strip 2 DIn   - bot right
//const int _ledDOut3Pin = 15;                  // DOut 3 -> LED strip 3 DIn   - bot left
//const int _ledDOut4Pin = 27;                  // DOut 4 -> LED strip 4 DIn   - SPARE ..for future use
//const int _ledDOut4Pin = 14;                  // DOut 4 -> LED strip 4 DIn   - SPARE
//const int _ledDOut4Pin = 13;                  // DOut 4 -> LED strip 4 DIn   - SPARE

const byte _pirPin = 12;                      // PIR sensor on interrupt pin (triggered on HIGH)

/*----------------------------modes---------------------------*/
const int _colorTempNum = 3;                  // 3 for now
String colorTempName[_colorTempNum] = { "Warm", "Standard", "CoolWhite" }; // color temperature sub-mode names for the main "Working" mode.

// top lights
const int _topModeNum = 9;
const int _topModePresetSlotNum = 6;
int _topModePreset[_topModePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
volatile int _topModeCur = 3;                 // current mode in use - this is not the var you are looking for.. the one to change is _topModePresetSlotCur
int _topModePresetSlotCur = 2;                // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
String _topModeName[_topModeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
int _topSaveCurMode = 0;
int _topColorTempCur = 1;                     // current colour temperature
// bottom lights
const int _botModeNum = 9;
const int _botModePresetSlotNum = 6;
int _botModePreset[_botModePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
volatile int _botModeCur = 4;                 // current mode in use - this is not the var you are looking for.. try _topModePresetSlotCur
int _botModePresetSlotCur = 3;                // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
String _botModeName[_botModeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
int _botSaveCurMode = 0;
int _botColorTempCur = 1;                     // current colour temperature

/*----------------------------buttons-------------------------*/

/*----------------------------PIR----------------------------*/
const unsigned long _pirHoldInterval = 10000; //150000; // 15000=15 sec. 30000=30 sec. 150000=2.5 mins.
volatile unsigned long _pirHoldPrevMillis = 0;
volatile bool _PIRtriggeredTimerRunning = false; // is the hold timer in use?
// crash at boot with ISR not in IRAM error
//void ICACHE_RAM_ATTR pirInterrupt0();

/*----------------------------touch sensors-------------------*/
// MPR121 connected via I2C
Mpr121 mpr121;                                // init MPR121 on I2C
u16 touch_status_flag[CHANNEL_NUM] = { 0 };   // u16 = unsigned short

/*----------------------------LED-----------------------------*/
// might limit power draw even further if add usb charge ports to the system
// or use usb chips and change power draw if usb device attached and charging
// 137 x 0.02 = 2.74
#define MAX_POWER_DRAW 3000                   // limit power draw to 3.0A at 5v (with 6A power supply this gives us a bit of head room for board, lights etc.)
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay //100

typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;

const uint16_t _ledNum = 137;
const int _segmentTotal = 9;                  // total segments

LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 0, 1 },      //blank for level shifting hack and debug status
  { 1, 28, 28 },    //top right 1A
  { 29, 36, 8 },    //top right 1B
  { 37, 44, 8 },    //top right 2A
  { 45, 72, 28 },   //top right 2B - 72 not working
  { 73, 73, 1 },    //blank for level shifting hack and debug status
  { 74, 101, 28 },  //top left 1A
  { 102, 108, 7 },  //top left 1B
  { 109, 136, 28 }  //top left 2 (window)
};
                                              
CRGBArray<_ledNum> _leds;                     // master array - CRGBArray means can do multiple '_leds(0, 2).fadeToBlackBy(40);' as well as single '_leds[0].fadeToBlackBy(40);'

CRGBSet _ledsTopRight( _leds(ledSegment[1].first, ledSegment[4].last) );
CRGBSet _ledsTopLeft( _leds(ledSegment[6].first, ledSegment[8].last) );

const int _ledGlobalBrightness = 255;         // global brightness - use this to cap the brightness
// top lights
int _topLedGlobalBrightnessCur = 255;         // top lights current global brightness - adjust this one!
int _topLedBrightnessIncDecAmount = 10;       // the brightness amount to increase or decrease
uint8_t _topGHue2 = 0;                        // incremental cycling "base color", 0-100, converted to 0-1
uint8_t _topGHue2saved = 0;                   // used to revert color when going back to 'Normal' mode
unsigned long _topGHue2CycleMillis = 200UL;   // gHue loop update time (millis)
uint8_t _topGHue2CycleSaved = 50;             // 0-255 mapped to millis range
uint8_t _topGHue2CycleMultiplier = 4;         // (__gHue2CycleSaved * _gHue2CycleMultiplier) = (unsigned long) _gHue2CycleMillis
unsigned long _topGHue2PrevMillis;            // gHue loop previous time (millis)
// bottom lights  
int _botLedGlobalBrightnessCur = 255;         // bottom lights current global brightness - adjust this one!
int _botLedBrightnessIncDecAmount = 10;       // the brightness amount to increase or decrease
uint8_t _botGHue2 = 0;                        // incremental cycling "base color", 0-100, converted to 0-1
uint8_t _botGHue2saved = 0;                   // used to revert color when going back to 'Normal' mode
unsigned long _botGHue2CycleMillis = 200UL;   // gHue loop update time (millis)
uint8_t _botGHue2CycleSaved = 50;             // 0-255 mapped to millis range
uint8_t _botGHue2CycleMultiplier = 4;         // (_gHue2CycleSaved * _gHue2CycleMultiplier) = (unsigned long) _gHue2CycleMillis
unsigned long _botGHue2PrevMillis;            // gHue loop previous time (millis)

//CHSV startColor( 144, 70, 64 );
//CHSV endColor( 31, 71, 69 );
//CRGB startColor_RGB( 3, 144, 232 );
//CRGB endColor_RGB( 249, 180, 1 );

CRGB _glowColor(32, 32, 32);
CRGB _topMorningColor(255, 255, 0);
CRGB _topDayColor(135,206,235);
CRGB _topEveningColor(128, 64, 64);
CRGB _topNightColor(64, 64, 64);

// can only work out how to set global temp so using individual colours for now.
//#define TEMPERATURE_0 WarmFluorescent
//#define TEMPERATURE_1 StandardFluorescent
//#define TEMPERATURE_2 CoolWhiteFluorescent

// RGB colours for "Working" colour temperature sub-mode
CRGB _warmFluorescent(255, 244, 229);         // WarmFluorescent = 0xFFF4E5 - 0 K, 255, 244, 229
CRGB _standardFluorescent(244, 255, 250);     // StandardFluorescent = 0xF4FFFA - 0 K, 244, 255, 250
CRGB _coolWhiteFluorescent(212, 235, 255);    // CoolWhiteFluorescent = 0xD4EBFF - 0 K, 212, 235, 255

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;                           // initialise
uint32_t id_bridge1 = DEVICE_ID_BRIDGE1;
uint8_t _stationChannel = STATION_CHANNEL;

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG_COMMS) { Serial.printf("kitchenLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (_runonce == true) {
    publishStatusAll(false);
    _runonce = false;
  }
  if (DEBUG_COMMS) { Serial.printf("--> kitchenLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  //publish..
  if (DEBUG_COMMS) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG_COMMS) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG_COMMS) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}


/*----------------------------MAIN----------------------------*/
void setup() {
  
  // start serial regardless but control debug output from mqtt
  Serial.begin(115200);
  
  Serial.println();
  Serial.print(_progName);
  Serial.print(" v.");
  Serial.print(_progVers);
  Serial.println();
  Serial.print("..");
  Serial.println();
  
  delay(3000);                                // give the power, LED strip, etc. a couple of secs to stabilise
  
  loadSettings();
  delay(400);
  setupLEDs();                                // setup the LEDs
  delay(400);
  setupPIR();
  delay(400);
  setupUserInputs();                          // setup any direct user input eg. buttons
  delay(400);
  setupMesh();
  delay(400);
  
  //everything done? ok then..
  Serial.print(F("Setup done"));
  Serial.println("-----");
  Serial.print(F("Device Node ID is "));
  String s = String(mesh.getNodeId());
  Serial.println(s);
  Serial.println("-----");
  Serial.println("");
  
  delay(1500);
}

void loop() {

  mesh.update();
  
  if(_firstTimeSetupDone == false) {             
    publishStatusAll(false);
    _firstTimeSetupDone = true;      
    if (DEBUG_GEN) { Serial.print(F("firstTimeSetupDone  = true")); }
  }

  loopPir();
  loopUserInputs();                           // loop direct user input
  loopTopModes();                             // loop top light modes
  loopBotModes();                             // loop bottom light modes
  loopDebug();
  loopSaveSettings();
  //factoryReset();              //TODO         // Press and hold the button to reset to factory defaults
  loopLED();
}

/*----------------------------interrupt callbacks----------------------------*/
void pirInterrupt() {
  if (DEBUG_INTERRUPT) { Serial.println("pirInterrupt"); }
  pirInterruptPart2();
}

void pirInterruptPart2() {
  if (_dayMode == false) {
    _topOnOff = true;
  }
  publishSensorOn(true);
  _pirHoldPrevMillis = millis();              // store the current time (reset the timer)
  _PIRtriggeredTimerRunning = true;           // enable the timer loop in pir
}

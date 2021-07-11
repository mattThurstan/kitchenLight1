/*----------------------------display-------------------------*/

void setupLEDs() {

  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_DRAW);

  //FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(leds, ledSegment[0].first, ledSegment[4].last).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(_leds, ledSegment[0].first, ledSegment[4].last).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(_leds, ledSegment[5].first, ledSegment[8].last).setCorrection( TypicalSMD5050 );

  _topLedGlobalBrightnessCur = _ledGlobalBrightness;
  _botLedGlobalBrightnessCur = _ledGlobalBrightness;
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature
}

/*----------------------------loop led----------------------------*/
void loopLED() {
  topGHueRotate();                                // top lights hue rotate for effects mode
  botGHueRotate();                                // bootom lights hue rotate for effects mode
  
  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

/*----------------------------led helpers----------------------------*/

/*
 * Increment gHue by 0.1 every N milliseconds
 * Saved as a uint8_t (using 0-100) and converted to a float (0.0-1.0)
 */
 // top lights
void topGHueRotate() {
  if (_topModeCur == 8) {
    unsigned long topGHue2CurMillis = millis();
    if ( (unsigned long)(topGHue2CurMillis - _topGHue2PrevMillis) >= _topGHue2CycleMillis) {
      _topGHue2PrevMillis = millis(); //re-initilize Timer
      _topGHue2++;                                     // slowly cycle the "base color" through the rainbow
      //checkAndSetColorHSL_H();
    }
  }
}
// bottom lights
void botGHueRotate() {
  if (_botModeCur == 8) {
    unsigned long botGHue2CurMillis = millis();
    if ( (unsigned long)(botGHue2CurMillis - _botGHue2PrevMillis) >= _botGHue2CycleMillis) {
      _botGHue2PrevMillis = millis(); //re-initilize Timer
      _botGHue2++;                                     // slowly cycle the "base color" through the rainbow
      //checkAndSetColorHSL_H();
    }
  }
}

/*
 * Color temperature utils
 */
 // top lights
void setTopColorTemp(int i) {
  _topColorTempCur = i;
}
void cycleTopColorTemp() {
  _topColorTempCur += 1;
  if (_topColorTempCur >= _colorTempNum) 
  { _topColorTempCur = 0; }  // rollover
}
// bottom lights
void setBotColorTemp(int i) {
  _botColorTempCur = i;
}
void cycleBotColorTemp() {
  _botColorTempCur += 1;
  if (_botColorTempCur >= _colorTempNum) 
  { _botColorTempCur = 0; }  // rollover
}

/*
 * Golbal brightness utils
 */
 // top lights
void setTopGlobalBrightness(int gb) {
  //use this to achieve an override from the mesh, eg. to match levels
  _topLedGlobalBrightnessCur = gb;
  topBrightnessRolloverCatch();
}
void increaseTopBrightness() {
  _topLedGlobalBrightnessCur += _topLedBrightnessIncDecAmount;
  topBrightnessRolloverCatch();
}
void decreaseTopBrightness() {
  _topLedGlobalBrightnessCur -= _topLedBrightnessIncDecAmount;
  topBrightnessRolloverCatch();
}
void topBrightnessRolloverCatch() {
  if(_topLedGlobalBrightnessCur > 255) {
    _topLedGlobalBrightnessCur = 255;
  } else if(_topLedGlobalBrightnessCur < 0) {
    _topLedGlobalBrightnessCur = 0;
  }
}
// bottom lights
void setBotGlobalBrightness(int gb) {
  //use this to achieve an override from the mesh, eg. to match levels
  _botLedGlobalBrightnessCur = gb;
  botBrightnessRolloverCatch();
}
void increaseBotBrightness() {
  _botLedGlobalBrightnessCur += _botLedBrightnessIncDecAmount;
  botBrightnessRolloverCatch();
}
void decreaseBotBrightness() {
  _botLedGlobalBrightnessCur -= _botLedBrightnessIncDecAmount;
  botBrightnessRolloverCatch();
}
void botBrightnessRolloverCatch() {
  if(_botLedGlobalBrightnessCur > 255) {
    _botLedGlobalBrightnessCur = 255;
  } else if(_botLedGlobalBrightnessCur < 0) {
    _botLedGlobalBrightnessCur = 0;
  }
}

/*
 Check Segment Endpoints
 usage | showSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void showSegmentEndpoints() {
  if (DEBUG_OVERLAY) { 
    // top lights right
    _leds[ledSegment[1].first] = CRGB::Red;
    _leds[ledSegment[1].last] = CRGB::Red;

    _leds[ledSegment[2].first] = CRGB::Red;
    _leds[ledSegment[2].last] = CRGB::Red;
    
    _leds[ledSegment[3].first] = CRGB::Red;
    _leds[ledSegment[3].last] = CRGB::Red;
    
    _leds[ledSegment[4].first] = CRGB::Red;
    _leds[ledSegment[4].last] = CRGB::Red;

    // top lights left
    _leds[ledSegment[6].first] = CRGB::Red;
    _leds[ledSegment[6].last] = CRGB::Red;
    
    _leds[ledSegment[7].first] = CRGB::Red;
    _leds[ledSegment[7].last] = CRGB::Red;

    // top lights windows
    _leds[ledSegment[8].first] = CRGB::Red;
    _leds[ledSegment[8].last] = CRGB::Red;

    // bot lights right
    // bot lights left
  }
}

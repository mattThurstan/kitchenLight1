/*----------------------------setup pir----------------------------*/
void setupPIR() {
  if (DEBUG_GEN) { Serial.println("Setup PIR"); }
  //setup PIR pins and attach interrupts
  pinMode(_pirPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_pirPin), pirInterrupt0, RISING);
}

/*----------------------------loop pir----------------------------*/
// if _dayMode is set to TRUE (day) then the lights will not turn on (breathing is still seperate).

void loopPir()  {
  if (_PIRtriggeredTimerRunning) {
    //pir timer - flips lights state if not in day mode
    unsigned long pirHoldCurMillis = millis();    // get current time
    if( (unsigned long)(pirHoldCurMillis - _pirHoldPrevMillis) >= _pirHoldInterval ) {
      //when the time has expired, do this..
      //if ((_state == 1 || _state == 2) && _dayMode == false) {
      if (_topOnOff == true && _dayMode == false) {
        //_state = 3;
        _topOnOff = false;
        //if (DEBUG_INTERRUPT) { Serial.print("State = 3"); }
        if (DEBUG_INTERRUPT) { Serial.print("PIR off - _topOnOff = false"); }
      }
      //publishSensorTopOff(true);
      //publishSensorBotOff(true);
      publishSensorOff(true);
      _PIRtriggeredTimerRunning = false;                      // disable itself
    }
  }
  
//  if (_state == 0) {
//    //off
//    RgbColor color;
//    for (uint16_t i = 1; i < strip.PixelCount(); i++)
//    {
//        color = strip.GetPixelColor(i);
//        color.Darken(2); //uint8_t darkenBy
//        strip.SetPixelColor(i, color);
//    }
//  } else if (_state == 1) {
//    publishState(true);
//    publishBrightness(true);
//    publishRGB(true);
//    fadeOn();
//  } else if (_state == 2) {
//    //on
//    strip.ClearTo(_colorHSL, ledSegment[1].first, ledSegment[1].last); 
//  } else if (_state == 3) {
//    publishState(true);
//    fadeOff();
//  }
}

void fadeOn() {
  _stateSave = _state;                            // interrupt catch
  //_leds.fadeToBlackBy(4);                         // just in case
  for (byte i = ledSegment[1].first; i <= ledSegment[1].last; i++) {
    if (_state != _stateSave) { return; }       // interrupt catch
    fadeShowLEDs(ledSegment[1].first, i);
    if (i == ledSegment[1].last) { 
      _state = 2;
      if (DEBUG_INTERRUPT) { Serial.println("State = 2"); }
      return;
    }
  }
}

void fadeOff() {
  _stateSave = _state;                            // interrupt catch
  for (byte i = ledSegment[1].last; i >= ledSegment[1].first; i--) {
    if (_state != _stateSave) { return; }       // interrupt catch
    strip.ClearTo(_rgbBlack);                   // fade em all
    fadeShowLEDs(ledSegment[1].first, i);       // then switch back on the ones we want
    if (i == ledSegment[1].first) { 
      strip.SetPixelColor(1, _rgbBlack);        // turn off the last pixel before changing state
      _state = 0;
      if (DEBUG_INTERRUPT) { Serial.print("State = 0"); }
      return;
    }
  }
}

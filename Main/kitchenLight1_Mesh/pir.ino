/*----------------------------setup pir----------------------------*/
void setupPIR() {
  if (DEBUG_GEN) { Serial.println("Setup PIR"); }
  //setup PIR pins and attach interrupts
  pinMode(_pirPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_pirPin), pirInterrupt, RISING);
}

/*----------------------------loop pir----------------------------*/
// if _dayMode is set to TRUE (day) then the lights will not turn on (breathing is still seperate).

void loopPir()  {
  if (_PIRtriggeredTimerRunning) {
    //pir timer - flips lights state if not in day mode
    unsigned long pirHoldCurMillis = millis();    // get current time
    if( (unsigned long)(pirHoldCurMillis - _pirHoldPrevMillis) >= _pirHoldInterval ) {
      //when the time has expired, do this..
      if (_topOnOff == true && _dayMode == false) {
        _topOnOff = false;
        if (DEBUG_INTERRUPT) { Serial.print("PIR off - _topOnOff = false"); }
      }
      publishSensorOff(true);
      _PIRtriggeredTimerRunning = false;                      // disable itself
    }
  }
}

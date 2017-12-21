/*----------------------------PIR sensor----------------------------*/

void setupPir() {
  pinMode(_pirPin,INPUT);                     //setup PIR pin 
}

/*
 * PIR sensor loop
 * called from main loop
 */
void loopPir() {

  EVERY_N_MILLISECONDS(_loopPirInterval) {    //FastLED based non-blocking delay to update/display the sequence.
    if(_pirToggled == false) {
      byte state = digitalRead(_pirPin);
      if (state == 1) {
        _onOff = true;                          //..this is basic, expand later
        _pirToggled = true;
        _pirPrevMillis = millis();              //store the current time
        #ifdef DEBUG
          Serial.println("PIR triggered");
          digitalWrite(13, 1);                //TEMP
        #endif
      }
      else if (state == 0) {
        //_onOff = false;
        #ifdef DEBUG
          //Serial.println("PIR off");
        #endif
      }
      #ifdef DEBUG
        //digitalWrite(13, state);                //TEMP
      #endif
    }
  }
  
  if(_pirToggled == true) {
    long pirCurMillis = millis();             //get current time
    if((long) (pirCurMillis - _pirPrevMillis) >= _pirHoldTime) {  // if((unsigned long)..
      _onOff = false;                         //..this is basic, expand later
      _pirToggled = false;                    //reset
      #ifdef DEBUG
        Serial.println("PIR hold ended");
        digitalWrite(13, 0);                //TEMP
      #endif
    }
  }
  
}

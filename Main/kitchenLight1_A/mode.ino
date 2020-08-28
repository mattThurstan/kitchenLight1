/*----------------------------mode----------------------------*/

/*
 * 0 - glow     (static/moving)
 * 1 - sunrise  (static/moving)
 * 2 - morning  (static/moving)
 * 2 - day      (static/moving) (full bright/overcast/clearbluesky)
 * 4 - working  (static) (full bright) (colour temperature sub-mode)
 * 5 - evening  (static/moving)
 * 6 - sunset   (static/moving)
 * 7 - night    (static/moving)
 * 8 - changing (moving) (could use static to pause it?)
 * 
 * gradient fade (top/mid/bot) (all can invert) (overlay/fx?)
 * moveable/resizable bright bar (overlay/fx?) (might use brightness +/- intputs to move up/down)
 */

 /*
  * All 9 modes are implemented (just in case).
  * ..but normal opperation will only use 1 mode (4).
  */
  
void loopModes() {
  if (_onOff) {
    if(_modeCur == 0) {       mode0(); } 
    else if(_modeCur == 1) {  mode1(); }
    else if(_modeCur == 2) {  mode2(); }
    else if(_modeCur == 3) {  mode3(); }
    else if(_modeCur == 4) {  mode4(); }
    else if(_modeCur == 5) {  mode5(); }
    else if(_modeCur == 6) {  mode6(); }
    else if(_modeCur == 7) {  mode7(); }
    else if(_modeCur == 8) {  mode8(); }
  }
  else { //turn off - fade everything to black
    fadeToBlackBy( leds, _ledNum, 32); 
  } //END onOff else
}

/*----------------------------modes----------------------------*/
void mode0() {
  //name should be glow!
}

void mode1() {
  //name should be sunRise!
}

void mode2() {
  //name is morning!
}

void mode3() {
  //name is day!
}

void mode4() {
  //name is working!
  //full-bright
  //fill_solid( leds[i], _ledNum, CRGB::White);
  fill_gradient_RGB(leds, ledSegment[0].first, CRGB::White, ledSegment[0].last, CRGB::White );
  fill_gradient_RGB(leds, ledSegment[1].first, CRGB::White, ledSegment[1].last, CRGB::White );
  
  //add sub temperature modes later..
  /*
  if(_mode0_sub == 0) {
    FastLED.setTemperature( TEMPERATURE_0 ); //set first temperature  //..moved to setup ..might work ???
    //fill_solid( leds, _ledNum, TEMPERATURE_0 );
    leds[0] = TEMPERATURE_0; //show indicator pixel
  } else if(_mode0_sub == 1) {
    FastLED.setTemperature( TEMPERATURE_1 );
    //fill_solid( leds, _ledNum, TEMPERATURE_1 );
    leds[0] = TEMPERATURE_1;
  } else if(_mode0_sub == 2) {
    FastLED.setTemperature( TEMPERATURE_2 );
    //fill_solid( leds, _ledNum, TEMPERATURE_2 );
    leds[0] = TEMPERATURE_2;
  }
  */
}

void mode5() {
  //name is evening!
}

void mode6() {
  //name is sunSet!
}

void mode7() {
  //name is night!
}

void mode8() {
  //name is changing!
}


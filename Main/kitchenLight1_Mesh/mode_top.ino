/*----------------------------mode - top lights---------------*/
/*
 * 0 - glow     (static/moving)
 * 1 - sunrise  (static/moving)
 * 2 - morning  (static/moving)
 * 2 - day      (static/moving) (full bright/overcast/clearbluesky)
 * 4 - working  (static) (full bright) (colour temperature sub-mode)
 * 5 - evening  (static/moving)
 * 6 - sunset   (static/moving)
 * 7 - night    (static/moving)
 * 8 - effect   (moving) (could use static to pause it?)
 * 
 * gradient fade (top/mid/bot) (all can invert) (overlay/fx?)
 * moveable/resizable bright bar (overlay/fx?) (might use brightness +/- intputs to move up/down)
 */

 /*
  * All 9 modes are implemented (just in case).
  * ..but normal opperation will only use 1 mode (4).
  */
  
void loopTopModes() {
  if (_topOnOff) {
    if(_topModeCur == 0) {       topMode0(); } 
    else if(_topModeCur == 1) {  topMode1(); }
    else if(_topModeCur == 2) {  topMode2(); }
    else if(_topModeCur == 3) {  topMode3(); }
    else if(_topModeCur == 4) {  topMode4(); }
    else if(_topModeCur == 5) {  topMode5(); }
    else if(_topModeCur == 6) {  topMode6(); }
    else if(_topModeCur == 7) {  topMode7(); }
    else if(_topModeCur == 8) {  topMode8(); }
  }
  else { 
    //turn off - fade everything to black
    //fadeToBlackBy( leds, _ledNum, 32); 
    _ledsTopRight.fadeToBlackBy(32);
    _ledsTopLeft.fadeToBlackBy(32);
  }
}

/*----------------------------modes----------------------------*/
void topMode0() {
  //name is glow!
  _ledsTopRight = _glowColor;
  _ledsTopLeft = _glowColor;
}

void topMode1() {
  //name should be sunRise!
}

void topMode2() {
  //name is morning!
  // this is yellow at bot fade to blue at top.
  _ledsTopRight = _topMorningColor;
  _ledsTopLeft = _topMorningColor;
}

void topMode3() {
  //name is day!
  // this is green at bot fade to blue at top.
  _ledsTopRight = _topDayColor;
  _ledsTopLeft = _topDayColor;
}

void topMode4() {
  //name is working!
  if(_topColorTempCur == 0) {
    _ledsTopRight = _warmFluorescent;
    _ledsTopLeft = _warmFluorescent;
  } else if(_topColorTempCur == 1) {
    _ledsTopRight = _standardFluorescent;
    _ledsTopLeft = _standardFluorescent;
  } else if(_topColorTempCur == 2) {
    _ledsTopRight = _coolWhiteFluorescent;
    _ledsTopLeft = _coolWhiteFluorescent;
  }
}

void topMode5() {
  //name is evening!
  _ledsTopRight = _topEveningColor;
  _ledsTopLeft = _topEveningColor;
}

void topMode6() {
  //name is sunSet!
}

void topMode7() {
  //name is night!
  _ledsTopRight = _topNightColor;
  _ledsTopLeft = _topNightColor;
}

void topMode8() {
  //name is effect!
}

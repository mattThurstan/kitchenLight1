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
  
void loopBotModes() {
  if (_botOnOff) {
    if(_botModeCur == 0) {       botMode0(); } 
    else if(_botModeCur == 1) {  botMode1(); }
    else if(_botModeCur == 2) {  botMode2(); }
    else if(_botModeCur == 3) {  botMode3(); }
    else if(_botModeCur == 4) {  botMode4(); }
    else if(_botModeCur == 5) {  botMode5(); }
    else if(_botModeCur == 6) {  botMode6(); }
    else if(_botModeCur == 7) {  botMode7(); }
    else if(_botModeCur == 8) {  botMode8(); }
  }
  else { 
    //turn off - fade everything to black
    //fadeToBlackBy( leds, _ledNum, 32); 
    //_ledsBotRight.fadeToBlackBy(32);
    //_ledsBotLeft.fadeToBlackBy(32);
  }
}

/*----------------------------modes----------------------------*/
void botMode0() {
  //name is glow!
//  _ledsBotRight = _glowColor;
//  _ledsBotLeft = _glowColor;
}

void botMode1() {
  //name should be sunRise!
}

void botMode2() {
  //name is morning!
  // this is yellow at bot fade to blue at top.
//  _ledsBotRight = _botMorningColor;
//  _ledsBotLeft = _botMorningColor;
}

void botMode3() {
  //name is day!
  // this is green at bot fade to blue at top.
//  _ledsBotRight = _botDayColor;
//  _ledsBotLeft = _botDayColor;
}

void botMode4() {
  //name is working!
  if(_botColorTempCur == 0) {
//    _ledsBotRight = _warmFluorescent;
//    _ledsBotLeft = _warmFluorescent;
  } else if(_botColorTempCur == 1) {
//    _ledsBotRight = _standardFluorescent;
//    _ledsBotLeft = _standardFluorescent;
  } else if(_botColorTempCur == 2) {
//    _ledsBotRight = _coolWhiteFluorescent;
//    _ledsBotLeft = _coolWhiteFluorescent;
  }
}

void botMode5() {
  //name is evening!
//  _ledsBotRight = _botEveningColor;
//  _ledsBotLeft = _botEveningColor;
}

void botMode6() {
  //name is sunSet!
}

void botMode7() {
  //name is night!
//  _ledsBotRight = _botNightColor;
//  _ledsBotLeft = _botNightColor;
}

void botMode8() {
  //name is effect!
}

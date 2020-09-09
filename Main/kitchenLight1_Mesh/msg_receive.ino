/*----------------------------internal mesh messages - get passed to/from MQTT broker by bridge----------------------------*/
void receiveMessage(uint32_t from, String msg)
{
  uint8_t firstMsgIndex = msg.indexOf(':');
  String targetSub = msg.substring(0, firstMsgIndex);
  String msgSub = msg.substring(firstMsgIndex+1);
// top lights
  if (targetSub == "lights/top/light/switch") {
    if (msgSub == "ON") { _topOnOff = true; }
    else if (msgSub == "OFF") { _topOnOff = false; }
    publishTopState(true);
  }
  else if (targetSub == "lights/top/brightness/set"){
    uint8_t brightness = msgSub.toInt();
    if (brightness < 0 || brightness > 255) {
      // do nothing...
      return;
    } else {
      setTopGlobalBrightness(brightness);
      publishTopBrightness(true);
    }
  }
  else if (targetSub == "lights/top/mode") {
    if (msgSub == "Glow") 
    { _topModeCur = 0;
      _topModePresetSlotCur = 0; }
    else if (msgSub == "Morning") 
    { _topModeCur = 2;
      _topModePresetSlotCur = 1; }
    else if (msgSub == "Day") 
    { _topModeCur = 3;
      _topModePresetSlotCur = 2; }
    else if (msgSub == "Working") 
    { _topModeCur = 4;
      _topModePresetSlotCur = 3; }
    else if (msgSub == "Evening") 
    { _topModeCur = 5;
      _topModePresetSlotCur = 4; }
    else if (msgSub == "Night") 
    { _topModeCur = 7;
      _topModePresetSlotCur = 5; }
    else if (msgSub == "Effect") 
    { _topModeCur = 8; }

//    publishTopMode(true);                   // bouncing back really badly!!!!
  }
  // top sub-modes
  else if (targetSub == "lights/top/mode/coltemp") {
    if (msgSub == "Warm") 
    { setTopColorTemp(0); }
    else if (msgSub == "Standard") 
    { setTopColorTemp(1); }
    else if (msgSub == "CoolWhite") 
    { setTopColorTemp(2); }
    
    publishTopColorTemp(true);
  }
  else if (targetSub == "lights/top/mode/effect") {
    // 
    //publishTopEffect(true);
  }
  // publish top effect colours
  else if (targetSub == "lights/top/hue/cycle/set") {
    uint8_t hueCycleLoopTime = msgSub.toInt();
    if (hueCycleLoopTime < 0 || hueCycleLoopTime > 255) { return; } 
    else {
      _topGHue2CycleSaved = hueCycleLoopTime;
//      checkAndSetTopGHue2CycleMillis();
      publishTopGHue2Cycle(true);
    }
  }
// bottom lights
  else if (targetSub == "lights/bot/light/switch") {
    if (msgSub == "ON") { _botOnOff = true; }
    else if (msgSub == "OFF") { _botOnOff = false; }
    publishBotState(true);
  }
  else if (targetSub == "lights/bot/brightness/set"){
    uint8_t brightness = msgSub.toInt();
    if (brightness < 0 || brightness > 255) {
      // do nothing...
      return;
    } else {
      setBotGlobalBrightness(brightness);
      publishBotBrightness(true);
    }
  }
  else if (targetSub == "lights/bot/mode") {
    if (msgSub == "Glow") 
    { _botModeCur = 0;
      _botModePresetSlotCur = 0; }
    else if (msgSub == "Morning") 
    { _botModeCur = 2;
      _botModePresetSlotCur = 1; }
    else if (msgSub == "Day") 
    { _botModeCur = 3;
      _botModePresetSlotCur = 2; }
    else if (msgSub == "Working") 
    { _botModeCur = 4;
      _botModePresetSlotCur = 3; }
    else if (msgSub == "Evening") 
    { _botModeCur = 5;
      _botModePresetSlotCur = 4; }
    else if (msgSub == "Night") 
    { _botModeCur = 7;
      _botModePresetSlotCur = 5; }
    else if (msgSub == "Effect") 
    { _botModeCur = 8; }

//    publishBotMode(true);                   // bouncing back really badly!!!!
  }
  // bottom sub-modes
  else if (targetSub == "lights/bot/mode/coltemp") {
    if (msgSub == "Warm") 
    { setBotColorTemp(0); }
    else if (msgSub == "Standard") 
    { setBotColorTemp(1); }
    else if (msgSub == "CoolWhite") 
    { setBotColorTemp(2); }
    
    publishBotColorTemp(true);
  }
  else if (targetSub == "lights/bot/mode/effect") {
    // 
    //publishBotEffect(true);
  }
  // publish bottom effect colours
  else if (targetSub == "lights/bot/hue/cycle/set") {
    uint8_t hueCycleLoopTime = msgSub.toInt();
    if (hueCycleLoopTime < 0 || hueCycleLoopTime > 255) { return; } 
    else {
      _botGHue2CycleSaved = hueCycleLoopTime;
//      checkAndSetBotGHue2CycleMillis();
      publishBotGHue2Cycle(true);
    }
  }
//etc.
  else if (targetSub == "sunrise") {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) { /*start sunrise*/ }
    else if (msgSub == LIGHTS_OFF) { /*stop sunrise and revert to previous setting*/ }
  }
  else if (targetSub == "lights/top/sunrise") {
    // trigger only
    // note: the single mesh msg of 'sunrise' is used for synced global sunrise
    if (msgSub == LIGHTS_ON) { /*start sunrise*/ }
    else if (msgSub == LIGHTS_OFF) { /*stop sunrise and revert to previous setting*/ }
    //else if (msgSub == "receive a time for sunrise to happen at") { /*set sunrise time*/ }
  }
  else if (targetSub == "sunset") {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) { /*start sunset*/ }
    else if (msgSub == LIGHTS_OFF) { /*stop sunset and revert to previous setting*/ }
  }
  else if (targetSub == "lights/top/sunset") {
    // trigger only
    // note: the single mesh msg of 'sunset' is used for synced global sunset
    if (msgSub == LIGHTS_ON) { /*start sunset*/ }
    else if (msgSub == LIGHTS_OFF) { /*stop sunset and revert to previous setting*/ }
    //else if (msgSub == "receive a time for sunset to happen at") { /*set sunset time*/ }
  }
  /*
   * Breath : (noun) Refers to a full cycle of breathing. It can also refer to the air that is inhaled or exhaled.
   */
  else if (targetSub == "breath") {
    // trigger only (global synced)
    // note: the single mesh msg of 'breath' is used for synced global breathing
    if (msgSub == LIGHTS_ON) {
      _topIsBreathingSynced = true;                    // set sync to global
      _topIsBreathing = true;                          // start synced breathing
    }
    else if (msgSub == LIGHTS_OFF) {
      _topIsBreathing = false;                         // stop breathing
      _topIsBreathingSynced = false;                   // set sync to local
    }
    //publishTopBreath(false);
  }
  else if (targetSub == "lights/top/breath") {
    // trigger only (local)
    // note: the single mesh msg of 'breath' is used for synced global breathing
    if (msgSub == LIGHTS_ON) {
      _topIsBreathingSynced = false;                   // set sync to local
      _topIsBreathing = true;                          // start local breathing
    }
    else if (msgSub == LIGHTS_OFF) {
      _topIsBreathing = false;                         // stop breathing
      _topIsBreathingSynced = false;                   // set sync to local
    }
    //publishLightsBreath(false);
  }
  else if (targetSub == "lights/top/breath/xyz") {
    // msg will contain xyz coords for position within the house
    //
    //publishLightsBreathXyzSet(false);
  }
  else if (targetSub == "lights/top/breath/xyz/mode") {
    // set positional mode
    if (msgSub == "Independent") { }
    else if (msgSub == "Global") { }
    //publishLightsBreathXyzMode(false);
  }
  else if(targetSub == "debug/general/set") {
    if (msgSub == LIGHTS_ON) { DEBUG_GEN = true; } 
    else if (msgSub == LIGHTS_OFF) { DEBUG_GEN = false; }
    publishDebugGeneralState(false);
  }
  else if (targetSub == "debug/overlay/set") {
    if (msgSub == LIGHTS_ON) { DEBUG_OVERLAY = true; }
    else if (msgSub == LIGHTS_OFF) { DEBUG_OVERLAY = false; }
    publishDebugOverlayState(false);
  }
  else if (targetSub == "debug/meshsync/set") {
    if (msgSub == LIGHTS_ON) { DEBUG_MESHSYNC = true; }
    else if (msgSub == LIGHTS_OFF) { DEBUG_MESHSYNC = false; }
    publishDebugMeshsyncState(false);
  }
  else if(targetSub == "debug/comms/set") {
    if (msgSub == LIGHTS_ON) { DEBUG_COMMS = true; } 
    else if (msgSub == LIGHTS_OFF) { DEBUG_COMMS = false; }
    publishDebugCommsState(false);
  }
  // don't really need an ON msg but using just to sure it wasn't sent in error
  else if(targetSub == "debug/reset") { if (msgSub == ON) { doReset(); } }
  else if(targetSub == "debug/restart") 
  {
    uint8_t restartTime = msg.toInt();
    if (restartTime < 0 || restartTime > 255) { return; /* do nothing... */ } 
    else { doRestart(restartTime); }
  }
  else if(targetSub == "reset") { if (msgSub == ON) { doReset(); } }
  else if(targetSub == "restart") 
  {
    uint8_t restartTime = msg.toInt();
    if (restartTime < 0 || restartTime > 255) { return; /* do nothing... */ } 
    else { doRestart(restartTime); }
  }
  else if(targetSub == "lockdown") 
  {
    uint8_t severity = msg.toInt();
    if (severity < 0 || severity > 255) { return; /* do nothing... */ } 
    else { doLockdown(severity); }
  }
  else if(targetSub == "status/request") { if (msgSub == ON) { publishStatusAll(false); }  }
  
  if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
}

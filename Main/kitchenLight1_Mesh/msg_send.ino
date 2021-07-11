/*----------------------------messages - publish-----------------------*/
void meshSendSingleToBridge(String nom, String msg, bool save) {
  if (DEBUG_COMMS) { Serial.print(nom); Serial.print(" - "); }
  mesh.sendSingle(id_bridge1, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { _shouldSaveSettings = true; }
}
void publishMeshMsgSingleState(String nom, String addr, boolean state, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  if (state == false) {  addr += "OFF"; }
  else if (state == true) { addr += "ON"; }
  meshSendSingleToBridge(nom, addr, save);
}
void publishMeshMsgSingleString(String nom, String addr, String msg, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  addr += msg;
  meshSendSingleToBridge(nom, addr, save);
}
void publishMeshMsgSingleColor(String nom, String addr, uint8_t r, uint8_t g, uint8_t b, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  addr += String(r);
  addr += ",";
  addr += String(g);
  addr += ",";
  addr += String(b);
  meshSendSingleToBridge(nom, addr, save);
}

/*----------------------------messages - publish - main - top lights---*/
void publishTopState(bool save) {
  publishMeshMsgSingleState("publishTopState", "lights/top/light/status", _topOnOff, save);
}
void publishTopBrightness(bool save) {
  publishMeshMsgSingleString("publishTopBrightness", "lights/top/brightness/status", String(_topLedGlobalBrightnessCur), save);
}

void publishTopMode(bool save) {
  publishMeshMsgSingleString("publishTopMode", "lights/top/mode", _topModeName[_topModeCur], save);
}

void publishDayMode(bool save) {
  publishMeshMsgSingleState("publishDayMode", "lights/day/status", _dayMode, save);
}

// PIR
void publishSensor(bool save) {
  //bool onOff;
  //if (_state == 0 || _state == 3) { onOff = false; } 
  //else { onOff = true; }
  publishMeshMsgSingleState("publishSensor", "sensor/status", _topOnOff, save);
}

void publishSensorOn(bool save) {
  publishMeshMsgSingleState("publishSensor", "sensor/status", true, save);
}
void publishSensorOff(bool save) {
  publishMeshMsgSingleState("publishSensor", "sensor/status", false, save);
}


// sub-modes
void publishTopColorTemp(bool save) { }
void publishTopEffect(bool save) { }

// publish top effect colours
  
void publishTopGHue2Cycle(bool save) {
  publishMeshMsgSingleString("publishTopGHue2Cycle", "lights/top/hue/cycle/status", String(_topGHue2CycleSaved), save);
}

/*----------------------------messages - publish - main - bottom lights*/
void publishBotState(bool save) {
  publishMeshMsgSingleState("publishBotState", "lights/bot/light/status", _botOnOff, save);
}
void publishBotBrightness(bool save) {
  publishMeshMsgSingleString("publishBotBrightness", "lights/bot/brightness/status", String(_botLedGlobalBrightnessCur), save);
}

void publishBotMode(bool save) {
  publishMeshMsgSingleString("publishBotMode", "lights/bot/mode", _botModeName[_botModeCur], save);
}

// sub-modes
void publishBotColorTemp(bool save) { }
void publishBotEffect(bool save) { }

// publish top effect colours
  
void publishBotGHue2Cycle(bool save) {
  publishMeshMsgSingleString("publishBotGHue2Cycle", "lights/bot/hue/cycle/status", String(_botGHue2CycleSaved), save);
}

/*----------------------------messages - publish - debug---------------*/
void publishDebugGeneralState(bool save) {
  publishMeshMsgSingleState("publishDebugGeneralState", "debug/general/status", DEBUG_GEN, save);
}

void publishDebugOverlayState(bool save) {
  publishMeshMsgSingleState("publishDebugOverlayState", "debug/overlay/status", DEBUG_OVERLAY, save);
}

void publishDebugMeshsyncState(bool save) {
  publishMeshMsgSingleState("publishDebugMeshsyncState", "debug/meshsync/status", DEBUG_MESHSYNC, save);
}

void publishDebugCommsState(bool save) {
  publishMeshMsgSingleState("publishDebugCommsState", "debug/comms/status", DEBUG_COMMS, save);
}

/*----------------------------messages - publish - main - all----------*/

void publishTopStatusAll(bool save) {
  
  if (DEBUG_COMMS) { Serial.println("publishTopStatusAll "); }
  publishTopState(save);
  publishTopBrightness(save);
  publishTopMode(save);
  publishTopColorTemp(save);
  publishTopEffect(save);
  // publish top effect colours
  publishTopGHue2Cycle(save);
  
  publishDayMode(save);
  publishSensor(save);
}

void publishBotStatusAll(bool save) {
  
  if (DEBUG_COMMS) { Serial.println("publishTopStatusAll "); }
  publishBotState(save);
  publishBotBrightness(save);
  publishBotMode(save);
  publishBotColorTemp(save);
  publishBotEffect(save);
  // publish bot effect colours
  publishBotGHue2Cycle(save);
  
  publishDayMode(save);
  publishSensor(save);
}

void publishDebugStatusAll(bool save) {
  publishDebugGeneralState(save);
  publishDebugOverlayState(save);
  publishDebugMeshsyncState(save);
  publishDebugCommsState(save);
}

void publishStatusAll(bool save) {
  
  if (DEBUG_COMMS) { Serial.println("publishStatusAll "); }
  publishTopStatusAll(save);
  publishBotStatusAll(save);
  
  publishDebugStatusAll(save);
}

void publishDeviceOffline() {
  publishMeshMsgSingleState("publishState", "status", OFF, false);
  publishMeshMsgSingleState("publishState", "available", "offline", false);
}

/*----------------------------util----------------------------*/
void turnOffWifi() {
  if (DEBUG_GEN) { Serial.println("Disconnecting wifi..."); }
  WiFi.disconnect();
}

void turnOffMesh() {
  if (DEBUG_GEN) { Serial.println("Disconnecting mesh..."); }
  mesh.stop();
}

void turnOffSerial() {
  if (DEBUG_GEN) { Serial.println("Disconnecting serial..."); }
  //
}

void factoryReset() { /* TODO */ }

void deviceRestart() { ESP.restart(); }

void deviceShutdown() { /* ??? */ }

/*----------------------------main calls-----------------------*/
/* Reset everything to default. */
void doReset() {
  resetDefaults();
  deviceRestart();
}

/* Restart the device (with a delay) */
void doRestart(uint8_t restartTime) {
  uint16_t dly = (restartTime * 60 * 1000); // static ???
  delay(dly);
  deviceRestart();
}

/*
 * Lockdown.
 * 
 * Emergency global disconnect (requires hard reset). 
 *  0 = do thing (no severity)
 *  1 = disconnect from LAN (bridges shutdown and mesh reboots) 
 *  2 = shutdown mesh (everything reboots in standalone mode) 
 *  3 = shutdown everything. 
 *      - Devices to power off if possible, if not then reboot in standalone emergency mode. 
 *      - These devices will require a hardware reset button implemented to clear the emergency mode.)
 */
void doLockdown(uint8_t severity) {
  if (severity < 4) { 
    LOCKDOWN_SEVERITY = severity;
    if (severity > 0) { LOCKDOWN = true; }
  }
  
  if (severity == 0) { /* do nothing */ } 
  else if (severity == 1) {
    turnOffComms();
    // set to restart with no comms
    doRestart(0);
  } else if (severity == 2) {
    turnOffComms();
    // set to restart in standalone mode
    doRestart(0);
  } else if (severity == 3) {
    turnOffComms();
    //set emergency mode
    //set lockdown severity (includes restart)
    //deviceShutdown(); // if possible ???
    // if not then set to restart in standalone emergency mode
    deviceRestart();
  } 
  else { }
}

/*
 * Debug utils
 */
void loopDebug() {
  if (DEBUG_OVERLAY) {
    showSegmentEndpoints();                         // overlay debug
    if (DEBUG_MESHSYNC) { }
  }
}

/*
 * Mode utils
 */
 // top lights
void incrementTopPresetSlot() {
  _topModePresetSlotCur += 1;
  incDecTopPresetSlot_prtB();
}
void decrementTopPresetSlot() {
  _topModePresetSlotCur -= 1;
  incDecTopPresetSlot_prtB();
}
void incDecTopPresetSlot_prtB() {
  if(_topModePresetSlotCur >= _topModePresetSlotNum || _topModePresetSlotCur < 0) { 
    _topModePresetSlotCur = 0;                     //TEMP rollover catch
  }
  _topModeCur = _topModePreset[_topModePresetSlotCur];
}
// bottom lights
void incrementBotPresetSlot() {
  _botModePresetSlotCur += 1;
  incDecBotPresetSlot_prtB();
}
void decrementBotPresetSlot() {
  _botModePresetSlotCur -= 1;
  incDecBotPresetSlot_prtB();
}
void incDecBotPresetSlot_prtB() {
  if(_botModePresetSlotCur >= _botModePresetSlotNum || _botModePresetSlotCur < 0) { 
    _botModePresetSlotCur = 0;                     //TEMP rollover catch
  }
  _botModeCur = _botModePreset[_botModePresetSlotCur];
}

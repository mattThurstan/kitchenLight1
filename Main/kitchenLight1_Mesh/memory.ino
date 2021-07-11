/*----------------------------memory----------------------------*/

void loadSettings() {
  preferences.begin("lightcontrol", true);  // true = read-only

  int isInit = preferences.getInt("isInit", 0); // have the settings been initialised?
  if (isInit == 0) {
    preferences.end();
    saveSettings();
  } else {
    _topLedGlobalBrightnessCur = preferences.getInt("tLedGlobBriCur", 255);
    _botLedGlobalBrightnessCur = preferences.getInt("bLedGlobBriCur", 255);
    //_topGHue2 = 0; // can't save single uint8_t ???
    //_botGHue2 = 0;
    _stationChannel = preferences.getUChar("stationChannel", STATION_CHANNEL);
    
    preferences.end();
  }
}

void saveSettings() {
  preferences.begin("lightcontrol", false); // false = read/write

  preferences.putInt("isInit", 1);
  preferences.putInt("tLedGlobBriCur", _topLedGlobalBrightnessCur);
  preferences.putInt("bLedGlobBriCur", _botLedGlobalBrightnessCur);
  preferences.putUChar("stationChannel", _stationChannel);
  
  preferences.end();
}

void clearSettings() {
  preferences.begin("lightcontrol", false); // false = read/write
  preferences.clear();
  preferences.end();
}

void loopSaveSettings() {
  EVERY_N_SECONDS(60) {                       // too much ???
    if (_shouldSaveSettings == true)
    { 
      saveSettings(); 
      _shouldSaveSettings = false; 
    }
  }
}

void resetDefaults() {
  clearSettings();
  setDefaults();
  saveSettings();
}

void setDefaults() 
{
  DEBUG_GEN = false;
  DEBUG_OVERLAY = false;
  DEBUG_MESHSYNC = false;
  DEBUG_COMMS = false;
  DEBUG_INTERRUPT = false;
  DEBUG_USERINPUT = false;

  _firstTimeSetupDone = false;
  _dayMode = false;
  
  _topOnOff = false;
  _botOnOff = false;
  _topIsBreathing = false;  
  _botIsBreathing = false;
  _topIsBreathOverlaid = false;
  _botIsBreathOverlaid = false;
  _topIsBreathingSynced = false;
  _botIsBreathingSynced = false;
    
  _topModeCur = 3;
  _botModeCur = 4;
  _topModePresetSlotCur = 2;
  _botModePresetSlotCur = 3;
  _topColorTempCur = 1;
  _botColorTempCur = 1;

  _topLedGlobalBrightnessCur = 255;
  _botLedGlobalBrightnessCur = 255;
  _topGHue2 = 0;
  _botGHue2 = 0;
  
  _stationChannel = STATION_CHANNEL;
}

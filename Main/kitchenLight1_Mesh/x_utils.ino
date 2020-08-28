/*----------------------------util----------------------------*/
void factoryReset() { }


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

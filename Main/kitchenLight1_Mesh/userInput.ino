/*----------------------------user input----------------------------*/
boolean _touchActive = false;

void setupUserInputs() {
  s32 ret = 0;                                // s32 = int
  if(mpr121.begin() < 0)
  {
    _touchActive = false;
    if (DEBUG_USERINPUT) { Serial.println("Can't detect MPR121 device!!!!"); }
  }
  else
  {
    mpr121.set_globle_param(0xDB20);
    mpr121.set_debounce(0X44);
    //mpr121.set_threshold();
    _touchActive = true;
    if (DEBUG_USERINPUT) { Serial.println("MPR121 device init OK!"); }
  }
  delay(100);
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  if (_touchActive) {
    touchSensorsMPR121();
  }
}

/*---------------touch sensors MPR121--------------*/
void touchSensorsMPR121() {
  u16 result = 0;                             // u16 = unsigned short
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };       // u8 = unsigned char
  
  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  for(int i = 0; i < CHANNEL_NUM; i++)
  {
    if (result&(1<<i))                             /*key i is pressed!!*/
    {
      if(0 == touch_status_flag[i])             
      { 
        touch_status_flag[i] = 1;
        
        if (i == 0) {
              touch0pressed();
        } else if (i == 1 || i == 2 || i == 3 || i == 4 || i == 5) { 
          if (_topOnOff == false) {
            _topOnOff = true;  //if the lights are already off, then turn them on
            //don't need to do anything else as we are already in a mode etc.
            publishTopState(true);
          } else {
             if (i == 1) {
              touch1pressed();
            } else if (i == 2) {
              touch2pressed();
            } else if (i == 3) {
              touch3pressed();
            } else if (i == 4) {
              touch4pressed();
            } else if (i == 5) {
              touch5pressed();
            }
          } // end if else _topOnOff = false
        } else if (i == 6) {
              touch6pressed();
        } else if (i == 7 || i == 8 || i == 9 || i == 10 || i == 11) { 
          if (_botOnOff == false) {
            _botOnOff = true;  //if the lights are already off, then turn them on
            //don't need to do anything else as we are already in a mode etc.
            publishBotState(true);
          } else {
             if (i == 7) {
              touch7pressed();
            } else if (i == 8) {
              touch8pressed();
            } else if (i == 9) {
              touch9pressed();
            } else if (i == 10) {
              touch10pressed();
            } else if (i == 11) {
              touch11pressed();
            }
          } // end if else _topOnOff = false
        }
        
        if (DEBUG_USERINPUT) { 
          Serial.print("Touch ");
          Serial.print(i);
          Serial.println(" pressed.");
          
          Serial.print("filtered_data_buf ");
          Serial.println(filtered_data_buf[i]);
          Serial.print("baseline_buf ");
          Serial.println(baseline_buf[i]);
          
        }
      } // end if touch status flag 0
    } // end if result
    else
    {
      if(1 == touch_status_flag[i])
      {
        touch_status_flag[i] = 0;

        if (DEBUG_USERINPUT) { 
          Serial.print("Touch ");
          Serial.print(i);
          Serial.println(" released.");
          
          Serial.print("filtered_data_buf ");
          Serial.println(filtered_data_buf[i]);
          Serial.print("baseline_buf ");
          Serial.println(baseline_buf[i]);
          
        }
      } // end  if touch status flag 1
    } // end else result
  } // end for-loop
  //delay(50);                                // ???
}

/*---------------touch sensors MPR121 - pressed--------------*/
// 0 - 5 top lights
void touch0pressed() {                        // touch sensor 0 - top lights on/off
  _topOnOff = !_topOnOff;                     // flip the top lights
  publishTopState(true);
}
void touch1pressed() {                        // touch sensor 1 - top lights mode up
  incrementTopPresetSlot();
  publishTopMode(true);
}
void touch2pressed() {                        // touch sensor 2 - top lights mode down
  decrementTopPresetSlot();
  publishTopMode(true);
}
void touch3pressed() {                        // touch sensor 3 - top lights sub-mode cycle
  // sub-modes eg. cycle temperature modes (not implemented yet)
  if (_topModeCur == 4) { 
    cycleTopColorTemp();
    publishTopColorTemp(true); 
  } else if (_topModeCur == 8) {
  }
}
void touch4pressed() {                        // touch sensor 4 - top lights brightness up
  increaseTopBrightness();
  publishTopBrightness(true);
}
void touch5pressed() {                        // touch sensor 5 - top lights brightness down
  decreaseTopBrightness();
  publishTopBrightness(true);
}

// 6 - 11 bottom lights
void touch6pressed() {                        // touch sensor 6 - bot lights on/off
  _botOnOff = !_botOnOff;                     // flip the bottom lights
  publishBotState(true);
}
void touch7pressed() {                        // touch sensor 7 - bot lights mode up
  incrementBotPresetSlot();
  publishBotMode(true);
}
void touch8pressed() {                        // touch sensor 8 - bot lights mode down 
  decrementBotPresetSlot();
  publishBotMode(true);
}
void touch9pressed() {                        // touch sensor 9 - bot lights sub-mode cycle
  // sub-modes eg. cycle temperature modes (not implemented yet)
  if (_botModeCur == 4) { 
    cycleBotColorTemp();
    publishBotColorTemp(true); 
  } else if (_botModeCur == 8) {
  }
}
void touch10pressed() {                        // touch sensor 10 - bot lights brightness up
  increaseBotBrightness();
  publishBotBrightness(true);
}
void touch11pressed() {                        // touch sensor 11 - bot lights brightness down
  decreaseBotBrightness();
  publishBotBrightness(true);
}

/*---------------touch sensors MPR121 - released--------------*/

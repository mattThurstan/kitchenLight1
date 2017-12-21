/*----------------------------display----------------------------*/

void setupLEDs() {
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 5700);  //..don't know yet

  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(leds, ledSegment[0].first, ledSegment[0].total).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(leds, ledSegment[1].first, ledSegment[1].total).setCorrection( TypicalSMD5050 );

  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature
}

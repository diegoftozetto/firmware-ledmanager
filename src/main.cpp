#include "LedManager.h"

void setup() {
  Serial.begin(115200);

  ESP_LOGD("SETUP", "Ok.");

  LedManager.begin(GPIO_NUM_2);

  // put your setup code here, to run once:
  
}

void loop() {

  String text = Serial.readStringUntil('\n');

  if(text.compareTo("START") == 0)
  {
    LedSettings ledSettings;

    ledSettings.timeHigh = 1000;
    ledSettings.timeLow = 1000;

    if(LedManager.start(ledSettings))
      ESP_LOGD("START", "Start Ok.");
    else
      ESP_LOGD("START", "Start Error.");
  }
  else if(text.compareTo("STOP") == 0)
  {
    if(LedManager.stop())
      ESP_LOGD("STOP", "Stop Ok.");
    else
      ESP_LOGD("STOP", "Stop Error.");
  }
}


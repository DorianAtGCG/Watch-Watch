void setup() 
{ 
  if (DEBUG) {
    Serial.begin(115200);        
  }

  // Disable Wi-Fi to save power
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect(true);

  // Disable Bluetooth to save power
  btStop();
  esp_bt_controller_disable();
  
  // When waking from timer, set clock and go back to sleep.
  // TODO: does this even get triggered on wake up from sleep? 
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    reportWatchTime("timer_wake_before_correction");
    correctTime();    
    reportWatchTime("timer_wake_after_correction");
    Serial.flush();
    goToSleep();
  }
  
  MySerial1.begin(9600, SERIAL_8N1, SERIAL_RECEIVE_PIN);
  myTransfer.begin(MySerial1, true, Serial);
//debugOut("Wakeup: " + (String)esp_sleep_get_wakeup_cause());

  pinMode(BATTERY_SENSOR, INPUT);   

  for (int p = 0; p < LED_PIN_COUNT; p++) {
    pinMode(LED_PINS[p], INPUT);
  }

  rtc.offset = RTC_OFFSET; 

  // while (true) {
  //   testOnes();
  // }

  debugOut("SETUP");
}

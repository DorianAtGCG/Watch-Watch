void print_date_time(time_t dt) { //easy way to print date and time
  if (!DEBUG)
    return;

  char buf[40];
  sprintf(buf, "%02d/%02d/%4d %02d:%02d:%02d", month(dt), day(dt), year(dt), hour(dt), minute(dt), second(dt));
  Serial.println(buf);
}

void print_date_time() { //easy way to print date and time
  if (!Serial)
    return;

  char buf[40];
  sprintf(buf, "%02d/%02d/%4d %02d:%02d:%02d", rtc.getMonth() + 1, rtc.getDay(), rtc.getYear(), rtc.getHour(), rtc.getMinute(), rtc.getSecond());
  Serial.println(buf);
}

void reportWatchTime(const char* label) {
  if (!DEBUG || !Serial)
    return;

  time_t utc = rtc.getEpoch();
  time_t local = rtc.getLocalEpoch();

  char utcBuf[24];
  char localBuf[24];
  sprintf(utcBuf, "%04d-%02d-%02d %02d:%02d:%02d", year(utc), month(utc), day(utc), hour(utc), minute(utc), second(utc));
  sprintf(localBuf, "%04d-%02d-%02d %02d:%02d:%02d", year(local), month(local), day(local), hour(local), minute(local), second(local));

  Serial.print("WATCH_TIME ");
  Serial.print(label);
  Serial.print(" local=");
  Serial.print(localBuf);
  Serial.print(" utc=");
  Serial.print(utcBuf);
  Serial.print(" epoch=");
  Serial.print(utc);
  Serial.print(" local_epoch=");
  Serial.print(local);
  Serial.print(" offset_seconds=");
  Serial.println(rtc.offset);
}

void handleSerialMonitor() {
  if (!DEBUG || !Serial)
    return;

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 't' || c == 'T') {
      reportWatchTime("serial_request");
      lastSerialMonitor = millis();
    }
  }
}

void debugOut(String s) {
  if (!DEBUG)
    return;

  char buf[200];
  sprintf(buf, "[%02d/%02d/%4d %02d:%02d:%02d]  %s", rtc.getMonth() + 1, rtc.getDay(), rtc.getYear(), rtc.getHour(true), rtc.getMinute(), rtc.getSecond(), s);  
  Serial.println(buf);
}

void goToSleep() {
  //delay(4000);
  // Disable Wi-Fi to save power
  WiFi.mode(WIFI_OFF);
  WiFi.disconnect(true);

  // Disable Bluetooth to save power
  btStop();
  esp_bt_controller_disable();

  // Shut down all unused GPIO pins
  // GPIO2 = 0, GPIO8 = 8 and GPIO9 = 9 are strapping pins
  
  // TILT = GPIO_NUM_3 = 1
  // SERIAL_RECEIVE_PIN D3
  // const int unusedPins[] = {BATTERY_SENSOR, D2, D6, D7, D8, D9, D10,SERIAL_RECEIVE_PIN,TILT_BUTTON,BATTERY_SENSOR};
  // for (int i = 0; i < sizeof(unusedPins)/sizeof(unusedPins[0]); i++) {
  //   pinMode(unusedPins[i], OUTPUT);
  //   digitalWrite(unusedPins[i], LOW);
  // }
  
  esp_sleep_enable_timer_wakeup(1000000ULL * SLEEP_TIME); 
  esp_deep_sleep_enable_gpio_wakeup(BIT(TILT_BUTTON), (esp_deepsleep_gpio_wake_up_mode_t) ESP_GPIO_WAKEUP_GPIO_HIGH); // WAKE on tap
  esp_deep_sleep_enable_gpio_wakeup(BIT(SERIAL_RECEIVE_PIN), (esp_deepsleep_gpio_wake_up_mode_t) ESP_GPIO_WAKEUP_GPIO_HIGH); // WAKE on base set

  if (DEBUG) {
    Serial.end();
  }
  esp_deep_sleep_start();  
}

float getBatteryVoltage() {
  uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(BATTERY_SENSOR); // ADC with correction   
  }

  if (Vbatt == 0)
    return 0;

  float Vbattf = 2 * Vbatt / 16 / 1000.0;     // attenuation ratio 1/2, mV --> V
  //Serial.println(Vbattf, 3);
  return Vbattf;
}

int getBatteryNumber() {
  float bv = getBatteryVoltage();
  if (bv <= 0)
    return 0;

  debugOut("BATT:");
  debugOut((String) bv);
  int iBV = (int)(((bv - 3) / 1.2) * 6); // change 3.0v - 4.2v to 0 - 5
  
  if (iBV < 0)
    iBV = 0;

  return iBV;
}

unsigned long getEpochMillis() {
  return rtc.getEpoch() * 1000 + rtc.getMillis();
}

int minusHour(int h) {
  int mh = h - 1;
  if (mh == 0)
    mh = 12;
  return mh;
}

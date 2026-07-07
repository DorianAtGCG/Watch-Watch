/// !!! NO DEBUG OUT WHILE IN EVENT HANDLER !!!
void RX_Handler() {
  struct __attribute__((packed)) STRUCT {
    short offset = 0;
    time_t gmt = 0;
  } WatchTime;

  myTransfer.rxObj(WatchTime);  
  rtc.offset = ((long) WatchTime.offset) * 60 * 60; // use offset to display local time for hours/minutes
  RTC_OFFSET = rtc.offset; // save for after sleep
  prefs.putLong("offset", RTC_OFFSET);
  if (RTC_OFFSET != lastRtcOffset) {
    lastOffsetChange = millis();
  }
  lastRtcOffset = RTC_OFFSET;

  rtc.setTime(WatchTime.gmt);

  lastTransfer = millis(); // don't let watch go to sleep
}

void Tilt_Handler() {  
  detachInterrupt(digitalPinToInterrupt(TILT_BUTTON));
  TILTED = true;
}

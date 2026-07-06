int lastHour = 0;
int lastTens = 0;
int lastOnes = 0;

void setLastLights(int hour, int tens, int ones) {
  lastHour = hour;
  lastTens = tens;
  lastOnes = ones;
}

bool didLightsChange(int hour, int tens, int ones) {
  return lastHour == hour && lastTens == tens && lastOnes == ones;
}

bool showTime(int ms) {
  print_date_time();
  
  int hour = rtc.getHour(); // 1-12 based hour
  if (hour == 0)
    hour = 12;
  
  int minute = rtc.getMinute();
  
//  if (didLightsChange(hour, tens, ones))
    allLightsOff();

//  setLastLights(hour, tens, ones);

  for (int x = 0; x < ms; x += 20) {
    handleSerialMonitor();

    if(myTransfer.available() > 0)
    {
      return true;
    }

    HoursTensOn(hour);
    delay(5);
    HoursTensOff(hour);

    HoursOnesOn(hour);
    delay(5);
    HoursOnesOff(hour);

    MinutesTensOn(minute);
    delay(5);
    MinutesTensOff(minute);

    MinutesOnesOn(minute);
    delay(5);
    MinutesOnesOff(minute);
  }

  return false;
}

void showBattery(int bDelay) {
  int ones;  
  int iBV = getBatteryNumber();

  debugOut((String) iBV);

  allLightsOff();
  
  for (int p = 0; p <= iBV; p++) {
    MinutesTensOn(p * 10);
    delay(200);
    MinutesTensOff(p * 10);
  }

  MinutesTensOn(iBV * 10);
  delay(bDelay);
  MinutesTensOff(iBV * 10);  
}

void showBattery() {
  showBattery(SHOW_MODE_MS);
}

void showDate() {
  int month = rtc.getMonth() + 1;   
  int day = rtc.getDay();

  allLightsOff();

  for (int x = 0; x < SHOW_MODE_MS; x += 20) {
    HoursTensOn(month);
    delay(5);
    HoursTensOff(month);

    HoursOnesOn(month);
    delay(5);
    HoursOnesOff(month);

    MinutesTensOn(day);
    delay(5);
    MinutesTensOff(day);

    MinutesOnesOn(day);
    delay(5);
    MinutesOnesOff(day);
  }
}

void showMoonPhase() {
  allLightsOff();
  ShowPhase(phaseOfMoon(), SHOW_MODE_MS);  
}

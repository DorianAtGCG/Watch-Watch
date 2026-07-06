const float moonCycleInDays = 29.53059;

int Moons[8][16] {  
  // these are off by one... LED D12 = LEDs[11]
  {11,17,22,23,24,25,26,21,16,8,3,2,0,6,5,4}, // 0 = FULL
  {11,18,19,20,16,8,3,2,0,6,5,4}, // 1 = WANING GIBBOUS
  {11,12,13,14,15,16,8,3,2,0,6,5,4}, // 2 = QTR C|
  {11,7,9,1,16,8,3,2,0,6,5,4}, // 3 = WANING CRESCENT
  {11,22,24,26,16,3,0,5}, // 4 = NEW
  {11,17,22,23,24,25,26,21,16,20,19,18}, // 5 = WAXING CRESCENT
  {11,17,22,23,24,25,26,21,16,15,14,13,12}, // 6 = QTR D
  {11,17,22,23,24,25,26,21,16,1,9,7} // 7 = WAXING GIBBOUS
};

int MoonSizes[8] {16, 12, 13, 12, 8, 12, 13, 12};

// (p)hase, (t)ime in ms
void ShowPhase(int p, int t) {
  allLightsOff();
  int mp = 0;
  for (int i = 0; i < t; i++) {
    int led = Moons[p][mp];
    LED_ON(led);
    delay(1);
    LED_OFF(led);
    if (++mp > MoonSizes[p] - 1)
      mp = 0;
  }
}

int mapPctToPhase(float mp) {
  int phase = 0;

  // FULL MOON
  if (mp >= 98 || mp <= 2)
    phase = 0;
  
  // WANING GIBBOUS
  if (mp > 2 && mp < 23)
    phase = 1;

  // 3rd QUARTER MOON at 25%
  if (mp >= 23 && mp < 27)
    phase = 2;

  // WANING CRESCENT
  if (mp >= 27 && mp < 48)  
    phase = 3;

  // NEW MOON
  if (mp >= 48 && mp < 52)
    phase = 4;
  
  // WAXING CRESCENT
  if (mp >= 52 && mp < 73)
    phase = 5;
  
  // 1st QUARTER
  if (mp >= 73 && mp < 77)
    phase = 6;

  // WAXING GIBBOUS
  if (mp >= 77 && mp < 98)
    phase = 7;

  return phase;
}

int phaseOfMoon() {
  time_t dt = rtc.getLocalEpoch();
  print_date_time(dt);

  //  known FULL MOON was on 7/3/2023 at 12:38pm GMT
  tmElements_t knownFullTME;
  knownFullTME.Day = 3;
  knownFullTME.Month = 7;
  knownFullTME.Year = 2023 - 1970;
  knownFullTME.Hour = 12;
  knownFullTME.Minute = 38;
  time_t knownFullT = makeTime(knownFullTME);

  long elapsedSeconds = dt - knownFullT;
  debugOut("seconds:");
  debugOut((String) elapsedSeconds);
  float elapsedDays = elapsedSeconds / (60 * 60 * 24);
  debugOut("days: ");
  debugOut((String) elapsedDays);
  //float numMoons = elapsedDays / moonCycleInDays;
  float numMoons = elapsedSeconds / (moonCycleInDays * 60 * 60 * 24);
  debugOut("# moons:");
  debugOut((String) numMoons);
  float moon_percent = numMoons - floor(numMoons);
  debugOut("Moon %");
  float mp = moon_percent * 100;
  debugOut((String) (mp));
  
  int phase = mapPctToPhase(mp);

  return (phase);
}

void moonDance() {
  for (int i = 0; i < 8; i++) {
    ShowPhase(i, 1000);
  }
}
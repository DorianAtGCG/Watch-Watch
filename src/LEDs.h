byte LED_PINS[] = { D2, D6, D7, D5, D4, D10 }; 
const byte LED_PIN_COUNT = sizeof(LED_PINS);


// Each LED: HIGH + LOW pin
int LEDs[][2] {
  {D2,D4},    // LED D1 = 0
  {D4,D2},    // 2
  {D2,D7},    // 3
  {D7,D2},    // 4
  {D2,D6},    // 5
  {D6,D2},    // 6
  {D2,D5},    // 7
  {D5,D2},    // 8
  {D2,D10},   // 9
  {D10,D2},   // 10
  {D4,D7},    // 11
  {D7,D4},    // 12
  {D4,D6},    // 13
  {D6,D4},    // 14
  {D4,D5},    // 15
  {D5,D4},    // 16
  {D4,D10},   // 17
  {D10,D4},   // 18
  {D7,D6},    // 19
  {D6,D7},    // 20
  {D7,D5},    // 21
  {D5,D7},    // 22
  {D7,D10},   // 23
  {D10,D7},   // 24
  {D6,D5},    // 25
  {D5,D6},    // 26
  {D6,D10},   // 27
};


int Numbers[4][10] {
  {0}, // Hours: Tens (1)
  {9,10,1,2,3,4,5,6,7,8}, // Hours: Ones (0 - 9)
  {16,15,14,13,12,11}, // Minutes: Tens (0 - 5)
  {26,17,18,19,20,21,22,23,24,25} // Minutes: Ones (0 - 9)
};

void LED_ON(int LED) {
  pinMode(LEDs[LED][0], OUTPUT);
  pinMode(LEDs[LED][1], OUTPUT);
  digitalWrite(LEDs[LED][0], HIGH);
  digitalWrite(LEDs[LED][1], LOW);
}

void LED_OFF(int LED) {
  pinMode(LEDs[LED][0], INPUT);
  pinMode(LEDs[LED][1], INPUT);
}

void allLightsOff() {
  for (int i = 0; i < LED_PIN_COUNT; i++) {
    pinMode(LED_PINS[i], INPUT);    
  }
}

void HoursTensOn(int h) {
  if (h < 10)
    return;

  LED_ON(Numbers[0][0]);
}

void HoursTensOff(int h) {
  if (h < 10)
    return;

  LED_OFF(Numbers[0][0]);
}

void HoursOnesOn(int h) {
  int ones = h % 10;
  LED_ON(Numbers[1][ones]);
}

void HoursOnesOff(int h) {
  int ones = h % 10;
  LED_OFF(Numbers[1][ones]);
}

int GetTens(int m) {
  int tens = 0;

  if (m > 9) {
    tens = (m / 10U) % 10;
  }

  return tens;
}

void MinutesTensOn(int m) {  
  LED_ON(Numbers[2][GetTens(m)]);
}

void MinutesTensOff(int m) {
  LED_OFF(Numbers[2][GetTens(m)]);  
}

void MinutesOnesOn(int m) {
  LED_ON(Numbers[3][m % 10]);
}

void MinutesOnesOff(int m) {
  LED_OFF(Numbers[3][m % 10]);
}

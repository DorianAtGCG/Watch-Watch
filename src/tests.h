
void testHours() {
  for (int i = 1; i < 13; i++) {
    Serial.print("Hour: ");
    Serial.println(i);

    for (int x = 0; x < 1000; x += 10) {
      HoursTensOn(i);
      delay(5);
      HoursTensOff(i);

      HoursOnesOn(i);
      delay(5);
      HoursOnesOn(i);
    }
  }
  delay(2000);
}

void testTens() {
  for (int i = 0; i < 6; i++) {
    Serial.print("Tens: ");
    Serial.println(i);

    MinutesTensOn(i);
    delay(1000);
    MinutesTensOff(i);
  }
  delay(2000);
}

void testOnes() {
  for (int i = 0; i < 10; i++) {
    Serial.print("Ones: ");
    Serial.println(i);

    MinutesOnesOn(i);
    delay(3000);
    MinutesOnesOff(i);
  }
  delay(3000);
}

void testAll() {
  testHours();
  testTens();
  testOnes();
}


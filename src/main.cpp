// Watch.V20
// ?? Must pull GPIO8 (D8) to HIGH (3.3V) to write to Seeeduino if it's asleep??

#include <Arduino.h>
#include <WiFi.h>   // Include Wi-Fi library
#include <esp_bt.h> // Include Bluetooth library
#include <HardwareSerial.h>  // Required for ESP32 serial - default "Serial1" does not work
#include <Preferences.h>
#include "SerialTransfer.h"
#include <ESP32Time.h>
#include <TimeLib.h>
#include "globals.h"
#include "helpers.h"
#include "LEDs.h"
#include "setTime.h"
#include "moon.h"
#include "showModes.h"
#include "eventHandlers.h"
#include "tests.h"
#include "setup.h"


void loop()
{
#ifdef LED_TEST_MODE
  testEachLED();
  return;
#endif

  debugOut("v2 loop start");

  bool onBase = false;
  do {
    handleSerialMonitor();

    if (showTime(3000)) {
      RX_Handler();
    }
    onBase = /*MySerial1.available() ||*/ myTransfer.available() || (lastTransfer > 0 && (millis() - lastTransfer) < 5000) || (lastSerialMonitor > 0 && (millis() - lastSerialMonitor) < 5000);
  } while (onBase);
  //print_date_time();

  // Not on base or no longer on base. Wearer tapped to wake up.

  // wait for second tap to show the rest
  unsigned long tap = millis();
  TILTED = false;
  attachInterrupt(digitalPinToInterrupt(TILT_BUTTON), Tilt_Handler, FALLING);
  while (millis() - tap < 2000) {
    if (TILTED) {      
      showDate();
      showMoonPhase();
      showBattery();
      break;
    }
  }  
 
  detachInterrupt(digitalPinToInterrupt(TILT_BUTTON)); // did removing this wreck the bootloader?
  
  debugOut("SLEEP");
  //showBattery(3000); // in case we're trying to reprogram the board
  goToSleep();
}

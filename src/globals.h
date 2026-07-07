const bool DEBUG = true; 
#define TILT_BUTTON  GPIO_NUM_3
#define SLEEP_TIME 600  // seconds between clock corrections
#define BATTERY_SENSOR A0
#define SHOW_MODE_MS 3000
#define SERIAL_RECEIVE_PIN D3 // YES 'D3', NOT '3'

HardwareSerial MySerial1(1);
SerialTransfer myTransfer;
Preferences prefs;

unsigned long lastTransfer = 0;
unsigned long lastSerialMonitor = 0;

RTC_DATA_ATTR long RTC_OFFSET;
RTC_DATA_ATTR float RTC_CORRECTION_REMAINDER;
long lastRtcOffset = 0;
unsigned long lastOffsetChange = 0;
ESP32Time rtc(0);

bool TILTED = false;

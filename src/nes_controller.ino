#include <led.h>
#include <output.h>
#include <nescontroller.h>

const pin_t DEBUG = A5;
const pin_t RESTART_TEST = A0;

const unsigned int ALL_BUTTONS_PRESSED = 0b11111111;

bool CONTROLLER_CONNECTED = false;
bool CONTROLLER_FIRST_CONNECTED = false;
bool CONTROLLER_FIRST_DISCONNECTED = false;
bool TESTING = false;
bool DEBUG_MODE = false;

// Needs to persist between loops
unsigned int buttonResults = 0b00000000;

void setup() {
  Serial.begin(9600);

  nescontroller::setup();
  led::setup();

  pinMode(DEBUG, INPUT_PULLDOWN);
  pinMode(RESTART_TEST, INPUT_PULLDOWN);
}

void loop() {
  checkDebug();
  sendLatchPulse();
  unsigned int registerData = readRegister();
  checkIfControllerConnected(registerData);
  if (CONTROLLER_CONNECTED) checkResetPin();

  if (CONTROLLER_FIRST_CONNECTED && !TESTING) {
    beginTest();
  }

  if (CONTROLLER_CONNECTED && TESTING) {
    if (registerData != ALL_BUTTONS_PRESSED) buttonResults |= (~registerData & 0b11111111);
    printTestStatus(buttonResults);
    if (DEBUG_MODE) Serial.printf("DATA %d %d\n", buttonResults, ALL_BUTTONS_PRESSED);
    if (buttonResults == ALL_BUTTONS_PRESSED) endTest(true);
  }

  if (!CONTROLLER_CONNECTED && TESTING) {
    endTest(buttonResults == ALL_BUTTONS_PRESSED);
  }

  if (CONTROLLER_FIRST_DISCONNECTED && !TESTING) {
    resetLeds(2s);
    CONTROLLER_FIRST_DISCONNECTED = false;
  }

  if (DEBUG_MODE) logData(registerData);
  delayMicroseconds(100000);
}

void checkDebug() {
  if (digitalRead(DEBUG)) {
    DEBUG_MODE = true;
  } else {
    DEBUG_MODE = false;
  }
}

void checkResetPin() {
  if (digitalRead(RESTART_TEST)) beginTest();
}

void beginTest() {
  CONTROLLER_FIRST_CONNECTED = false;
  TESTING = true;
  buttonResults = 0b00000000;
  resetTestingLed();
}

void endTest(bool pass) {
  setLed(TESTING_LED, false);
  TESTING = false;

  if (pass) {
    Serial.println("***** TEST PASSED *****");
    setLed(PASS_LED, true);
  } else {
    Serial.println("***** TEST FAILED *****");
    setLed(FAIL_LED, true);
  }
}

void checkIfControllerConnected(unsigned int data) {
  if (data == 0b11111111 && !CONTROLLER_CONNECTED) {
    if (DEBUG_MODE) Serial.println("CONTROLLER CONNECTED");
    CONTROLLER_CONNECTED = true;
    CONTROLLER_FIRST_CONNECTED = true;
  }
  if (data == 0b00000000 && CONTROLLER_CONNECTED) {
    if (DEBUG_MODE) Serial.println("CONTROLLER DISCONNECTED");
    CONTROLLER_CONNECTED = false;
    CONTROLLER_FIRST_DISCONNECTED = true;
  }
}

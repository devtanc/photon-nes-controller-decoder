/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/devtanc/GitHub/particle/nes_controller/src/nes_controller_decoder.ino"
#include <led.h>
#include <output.h>
#include <nescontroller.h>
#include <sn74register.h>

void setup();
void checkResetPin();
void beginTest();
void endTest(bool pass);
void checkIfControllerConnected(unsigned int data);
void outputToLeds(unsigned int registerData);
void loop();
#line 6 "/Users/devtanc/GitHub/particle/nes_controller/src/nes_controller_decoder.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const pin_t RESTART_TEST = A0;
const int DISCONNECT_DELAY_MS = 2000;
const unsigned int ALL_BUTTONS_PRESSED = 0b11111111;

bool CONTROLLER_CONNECTED = false;
bool CONTROLLER_FIRST_CONNECTED = false;
bool CONTROLLER_FIRST_DISCONNECTED = false;
bool TESTING = false;

unsigned long controller_disconnect_time;

// Needs to persist between loops
unsigned int buttonResults = 0b00000000;

void setup() {
  Serial.begin(9600);

  nescontroller::setup();
  led::setup();
  sn74register::setup();

  pinMode(RESTART_TEST, INPUT_PULLDOWN);
}

void checkResetPin() {
  if (digitalRead(RESTART_TEST)) beginTest();
}

void beginTest() {
  CONTROLLER_FIRST_DISCONNECTED = false;
  CONTROLLER_FIRST_CONNECTED = false;
  TESTING = true;
  buttonResults = 0b00000000;
  led::testing();
}

void endTest(bool pass) {
  led::off();
  TESTING = false;

  if (pass) {
    Serial.println("***** TEST PASSED *****");
    led::success();
  } else {
    Serial.println("***** TEST FAILED *****");
    led::failure();
  }
}

void checkIfControllerConnected(unsigned int data) {
  if (data == 0b11111111 && !CONTROLLER_CONNECTED) {
    CONTROLLER_CONNECTED = true;
    CONTROLLER_FIRST_CONNECTED = true;
  }
  if (data == 0b00000000 && CONTROLLER_CONNECTED) {
    CONTROLLER_CONNECTED = false;
    CONTROLLER_FIRST_DISCONNECTED = true;
    controller_disconnect_time = millis();
  }
}

void outputToLeds(unsigned int registerData) {
  sn74register::disableOutput();
  sn74register::writeRegister(led::getTestingLeds(), MSBFIRST);
  sn74register::writeRegister(buttonResults, MSBFIRST);
  sn74register::writeRegister(registerData, MSBFIRST);
  sn74register::enableOutput();
}

void loop() {
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
    if (buttonResults == ALL_BUTTONS_PRESSED) endTest(true);
  }

  if (!CONTROLLER_CONNECTED && TESTING) {
    endTest(buttonResults == ALL_BUTTONS_PRESSED);
  }

  if (CONTROLLER_FIRST_DISCONNECTED && millis() > controller_disconnect_time + DISCONNECT_DELAY_MS) {
    led::off();
    CONTROLLER_FIRST_DISCONNECTED = false;
  }

  outputToLeds(registerData);
  
  delayMicroseconds(60);
}

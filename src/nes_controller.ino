#include <string>

const pin_t CLOCK = D0;
const pin_t LATCH = D1;
const pin_t DATA = D2;

const pin_t TESTING_LED = A1;
const pin_t PASS_LED = A2;
const pin_t FAIL_LED = A3;

const pin_t DEBUG = A5;
const pin_t RESTART_TEST = A0;

const int CLOCK_DELAY = 10;
const unsigned int ALL_BUTTONS_PRESSED = 0b11111111;

LEDStatus ledTesting(RGB_COLOR_BLUE, LED_PATTERN_FADE, LED_PRIORITY_NORMAL);
LEDStatus ledPass(RGB_COLOR_GREEN, LED_PATTERN_SOLID, LED_PRIORITY_NORMAL);
LEDStatus ledFail(RGB_COLOR_RED, LED_PATTERN_SOLID, LED_PRIORITY_NORMAL);

bool CONTROLLER_CONNECTED = false;
bool CONTROLLER_CONNECTED_FIRST_TIME = false;
bool TESTING = false;
bool DEBUG_MODE = false;

// Needs to persist between loops
unsigned int buttonResults = 0b00000000;

typedef enum {
  RELEASED,
  PRESSED
} ButtonState;

typedef enum {
  A,
  B,
  SELECT,
  START,
  UP,
  DOWN,
  LEFT,
  RIGHT
} Button;

void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, INPUT);

  pinMode(TESTING_LED, OUTPUT);
  pinMode(PASS_LED, OUTPUT);
  pinMode(FAIL_LED, OUTPUT);

  pinMode(DEBUG, INPUT_PULLDOWN);
  pinMode(RESTART_TEST, INPUT_PULLDOWN);

  digitalWrite(CLOCK, HIGH);
}

void loop() {
  checkDebug();
  sendLatchPulse();
  unsigned int registerData = readRegister();
  checkIfControllerConnected(registerData);
  if (CONTROLLER_CONNECTED) checkResetPin();

  if (CONTROLLER_CONNECTED_FIRST_TIME && !TESTING) {
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

  if (!CONTROLLER_CONNECTED && !TESTING) {
    delay(2s);
    clearLeds();
  }

  if (DEBUG_MODE) logData(registerData);
  delayMicroseconds(100000);
}

void clearLeds() {
  setLed(PASS_LED, false);
  setLed(FAIL_LED, false);
  setLed(TESTING_LED, false);
}

void resetTestingLed() {
  setLed(PASS_LED, false);
  setLed(FAIL_LED, false);
  setLed(TESTING_LED, true);
}

void setLed(pin_t LED, bool on) {
  if (on) {
    digitalWrite(LED, HIGH);
    switch (LED) {
      case TESTING_LED:
        ledTesting.setActive(true);
        break;
      case PASS_LED:
        ledPass.setActive(true);
        break;
      case FAIL_LED:
        ledFail.setActive(true);
        break;
      default:
        break;
    }
  } else {
    digitalWrite(LED, LOW);
    switch (LED) {
      case TESTING_LED:
        ledTesting.setActive(false);
        break;
      case PASS_LED:
        ledPass.setActive(false);
        break;
      case FAIL_LED:
        ledFail.setActive(false);
        break;
      default:
        break;
    }
  }
}

void checkResetPin() {
  if (digitalRead(RESTART_TEST)) beginTest();
}

void checkDebug() {
  if (digitalRead(DEBUG)) {
    DEBUG_MODE = true;
  } else {
    DEBUG_MODE = false;
  }
}

void beginTest() {
  CONTROLLER_CONNECTED_FIRST_TIME = false;
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
    CONTROLLER_CONNECTED_FIRST_TIME = true;
  }
  if (data == 0b00000000 && CONTROLLER_CONNECTED) {
    if (DEBUG_MODE) Serial.println("CONTROLLER DISCONNECTED");
    CONTROLLER_CONNECTED = false;
  }
}

int readRegister() {
  unsigned int data = 0b00000000;

  for (int i = 0; i < 8; i++) {
    int bit = digitalRead(DATA);
    data = setBit(data, i, bit);
    sendClockPulse();
  }

  return data;
}

void sendLatchPulse() {
  digitalWrite(LATCH, HIGH);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(LATCH, LOW);
  delayMicroseconds(CLOCK_DELAY);
}

void sendClockPulse() {
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK, LOW);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK, HIGH);
  delayMicroseconds(CLOCK_DELAY);
}

int setBit(unsigned int data, int bit, unsigned int value) {
  if (value == 0) return data;

  switch(bit) {
    case A:
      return data | 0b00000001;
    case B:
      return data | 0b00000010;
    case SELECT:
      return data | 0b00000100;
    case START:
      return data | 0b00001000;
    case UP:
      return data | 0b00010000;
    case DOWN:
      return data | 0b00100000;
    case LEFT:
      return data | 0b01000000;
    case RIGHT:
      return data | 0b10000000;
    default:
      return data;
  }
}

void createBinaryString(unsigned int data, char result[]) {
  for (int i = 0; i < 8; i++) {
    if (data & 1) {
      // 7 - i makes sure that we keep the bits in the right order
      result[7 - i] = '1';
    } else {
      result[7 - i] = '0';
    }
    data >>= 1;
  }
  result[8] = '\0';
}

void clearConsole() {
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void logData(unsigned int data) {
  char dataString[9];
  createBinaryString(data, dataString);
  Serial.println(dataString);
}

void printTestStatus(unsigned int buttonBits) {
  clearConsole();
  Serial.println("\n*** TEST STATUS ***");
  Serial.print("Button A:\t\t");
  printBitStatus(buttonBits, 0b00000001);
  Serial.print("Button B:\t\t");
  printBitStatus(buttonBits, 0b00000010);
  Serial.print("Button Select:\t\t");
  printBitStatus(buttonBits, 0b00000100);
  Serial.print("Button Start:\t\t");
  printBitStatus(buttonBits, 0b00001000);
  Serial.print("Button Up:\t\t");
  printBitStatus(buttonBits, 0b00010000);
  Serial.print("Button Down:\t\t");
  printBitStatus(buttonBits, 0b00100000);
  Serial.print("Button Left:\t\t");
  printBitStatus(buttonBits, 0b01000000);
  Serial.print("Button Right:\t\t");
  printBitStatus(buttonBits, 0b10000000);
}

void printBitStatus(unsigned int data, unsigned int bitmap) {
  if (data & bitmap) {
    Serial.println("GOOD");
  } else {
    Serial.println("");
  }
}
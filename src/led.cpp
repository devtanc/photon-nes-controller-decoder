#include <led.h>

namespace led {
  unsigned int testingLeds = 0b00000000;

  void setup() {
    testingLeds = NOTHING;
  }

  void off() {
    testingLeds = NOTHING;
  }

  void testing() {
    testingLeds = TESTING;
  }

  void success() {
    testingLeds = SUCCESS;
  }

  void failure() {
    testingLeds = FAILURE;
  }

  unsigned int getTestingLeds() {
    return testingLeds;
  }
}
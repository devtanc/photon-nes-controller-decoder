#ifndef LED_H
#define LED_H

namespace led {
    const int NOTHING = 0b00000000;
    const int SUCCESS = 0b00000001;
    const int TESTING = 0b00000010;
    const int FAILURE = 0b00000100;

    void setup();
    void off();
    void testing();
    void success();
    void failure();
    unsigned int getTestingLeds();
}

#endif /* LED_H */
#include <Particle.h>

namespace nescontroller {
    void setup();
}

int readRegister();
void sendLatchPulse();
void sendClockPulse();
int setBit(unsigned int data, int bit, unsigned int value);
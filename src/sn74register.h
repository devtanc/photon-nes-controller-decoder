#ifndef SN74REGISTER_H
#define SN74REGISTER_H

#include <Particle.h>

namespace sn74register {
    void setup();

    const pin_t DATA = D4; // SER
    const pin_t ENABLE_OUTPUT = D3; // OE
    const pin_t LATCH = D2; // RCLK
    const pin_t SHIFT_CLOCK = D1; // SRCLK
    const pin_t CLEAR = D0; // SRCLR
    
    void writeRegister(int data, int method);
    void enableOutput();
    void disableOutput();
}


#endif /* SN74REGISTER_H */
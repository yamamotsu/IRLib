#ifndef _IR_LED_H_
#define _IR_LED_H_

#include "ir_data.h"

class IRLed
{
private:
    byte _pin_led;
    byte _high, _low;
public:
    IRLed(byte pin_led, bool invert_out = false);
    void output(byte bit,
        long howlong_us,
        int career_high_us = 9,
        int career_low_us = 18);

    int sendIRData(IRData &data);
};

#endif //_IR_LED_H_

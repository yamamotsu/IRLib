#ifndef _IR_RECEIVER_H_
#define _IR_RECEIVER_H_

#include "ir_wave.h"

class IRReceiver
{
private:
    byte _pin;
    //unsigned int _sampling_period;
    byte _input_high, _input_low;
public:
    IRReceiver(byte pin_receiver, bool invert_out = false);
    byte input();
    void getIR(IRWave &wave, long samples = -1);
};

#endif //_IR_RECEIVER_H_

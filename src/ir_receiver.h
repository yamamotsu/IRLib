#ifndef _IR_RECEIVER_H_
#define _IR_RECEIVER_H_

#include "ir_wave.h"
#include "ir_data.h"

class IRReceiver
{
public:
    enum IRStatus
    {
        stop = 0,
        signal_detected,
        leader_detected,
        repeat_detected
    };
private:
    byte _pin;
    //unsigned int _sampling_period;
    byte _input_high, _input_low;
    IRStatus _status;
    int _waitSignal(long timeout_ms = -1);
    int _scanLeaderCode(IRData &data, long high_time, long low_time, bool format_autodetect = true);
    int _getDataBit(long high_time, long low_time);
public:
    IRReceiver(byte pin_receiver, bool invert_in = false);
    byte input();
    void getIR(IRWave &wave, long samples = -1);
    int getIRData(IRData &data, long timeout_ms = -1);
};

#endif //_IR_RECEIVER_H_

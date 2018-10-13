#include "ir_receiver.h"

IRReceiver::IRReceiver(byte pin_receiver, bool invert_out)
{
    _pin = pin_receiver;
    if(invert_out)
    {
        _input_high = 0;
        _input_low = 1;
    }
    else
    {
        _input_high = 1;
        _input_low = 0;
    }

    // set led pin mode
    pinMode(_pin, INPUT);
}

byte IRReceiver::input()
{
    byte in = digitalRead(_pin);
    if(in == HIGH)
    {
        return _input_high;
    }
    else
    {
        return _input_low;
    }

}

void IRReceiver::getIR(IRWave &wave, long samples)
{
    unsigned int delta_time = wave.deltaMicros();
    Serial.println(delta_time);
    //unsigned long start_t = millis(), end_t;
    unsigned long tlim;
    long sample_num = (samples == -1 ? (wave.buffer_bytes() << 3) : samples);
    byte value;
    for(auto i = 0; i < sample_num; i++)
    {
        tlim = micros() + delta_time;
        value = input();
        wave.append(value);
        while(micros() < tlim) delayMicroseconds(1);
    }
}

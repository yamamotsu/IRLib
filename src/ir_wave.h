#ifndef _IR_WAVE_H_
#define _IR_WAVE_H_

const unsigned long _wave_buf_bytes = 1000;

class IRWave
{
private:
    byte _buffer[_wave_buf_bytes];
    long _in_itr_byte, _out_itr_byte;
    byte _in_itr_bit, _out_itr_bit;
    int _sampling_period;
    bool _is_empty;
    void _SwpNextOutItr();
    void _SwpNextInItr();
public:
    IRWave(int sampling_period_us = 20);
    ~IRWave();
    const long samples();
    const int deltaMicros();
    const unsigned long buffer_bytes();
    const int get();
    int append(byte value, bool allow_overwrite = true);
};

#endif //_IR_WAVE_H_

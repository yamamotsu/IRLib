#include <arduino.h>
#include "ir_wave.h"

IRWave::IRWave(int sampling_period_us)
{
    _in_itr_byte = 0;
    _in_itr_bit = 7;
    _out_itr_byte = 0;
    _out_itr_bit = 7;
    _sampling_period = sampling_period_us;
    _is_empty = true;
}

IRWave::~IRWave()
{

}

const long IRWave::samples()
{
    long in_bits = (_in_itr_byte << 3) | _in_itr_bit;
    long out_bits = (_out_itr_byte << 3) | _out_itr_bit;
    long samples = out_bits - in_bits;
    Serial.print("out_bits: "); Serial.println(out_bits);
    Serial.print("in_bits: "); Serial.println(in_bits);
    Serial.print("samples: "); Serial.println(samples);
    if(samples >= 0)
    {
        return samples;
    }
    else
    {
        return (_wave_buf_bytes << 3) + samples;
    }
}

const unsigned long IRWave::buffer_bytes()
{
    return _wave_buf_bytes;
}

void IRWave::_SwpNextOutItr()
{
    if(_out_itr_bit == 0)
    {
        _out_itr_bit = 7;
        if(_out_itr_byte == 0)
        {
            _out_itr_byte = _wave_buf_bytes - 1;
        }
        else
        {
            _out_itr_byte--;
        }
    }
    else
    {
        _out_itr_bit--;
    }
}

void IRWave::_SwpNextInItr()
{
    if(_in_itr_bit == 0)
    {
        _in_itr_bit = 7;
        if(_in_itr_byte == 0)
        {
            _in_itr_byte = _wave_buf_bytes - 1;
        }
        else
        {
            _in_itr_byte--;
        }
    }
    else
    {
        _in_itr_bit--;
    }
}

const int IRWave::deltaMicros()
{
    return _sampling_period;
}

const int IRWave::get()
{
    if(_is_empty)
    {
        return -1;
    }

    byte value = (_buffer[_out_itr_byte] >> _out_itr_bit) & 0x01;

    _SwpNextOutItr();

    if(_out_itr_byte == _in_itr_byte && _out_itr_bit == _in_itr_bit)
    {
        _is_empty = true;
    }

    return value;
}

int IRWave::append(byte value, bool allow_overwrite)
{
    if(_out_itr_byte == _in_itr_byte && _out_itr_bit == _in_itr_bit)
    {
        if(_is_empty)
        {
            _is_empty = false;
        }
        else
        {
            if(allow_overwrite)
            {
                _SwpNextOutItr();
            }
            else
            {
                return -1;
            }
        }
    }

    _buffer[_in_itr_byte] |= ((value & 0x01) << _in_itr_bit);

    _SwpNextInItr();

    return 0;
}

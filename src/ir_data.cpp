#include <arduino.h>
#include "ir_data.h"

IRData::IRData()
    :_buffer({0}), _data_len_bits(0)
{
    format = IRDataFormat::AUTO;
    is_repeat_code = false;
}

const int IRData::capacity()
{
    return sizeof(_buffer);
}

const int IRData::bitLengh()
{
    return _data_len_bits;
}

void IRData::clear()
{
    unsigned int lim = capacity();
    for(auto i=0; i < lim; i++)
    {
        _buffer[i] = 0;
    }
    _data_len_bits = 0;
    is_repeat_code = false;
}

long IRData::append(byte databit)
{
    long byte_idx = _data_len_bits >> 3;
    if(_data_len_bits < (capacity() << 3))
    {
        _buffer[byte_idx] = (_buffer[byte_idx] << 1) | (databit & 0x01);
        return ++_data_len_bits;
    }
    return -1;
}

const int IRData::get(int index)
{
    if(index >= 0 && index < bitLengh())
    {
        return _buffer[index];
    }
    else if(index < 0 && index >= -bitLengh())
    {
        return _buffer[bitLengh() + index];
    }
    return -1;
}

const int IRData::operator[] (int index)
{
    return get(index);
}

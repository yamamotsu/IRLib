#include <arduino.h>
#include "ir_data.h"

IRData::IRData()
    :_buffer({0})
{
    _data_len_bits = 0;
    format = IRDataFormat::AUTO;
    is_repeat_code = false;
    _bit_itr = 0;
}

IRData::IRData(String data_hex_str, IRDataFormat format)
    :_buffer({0})
{
    setDataByString(data_hex_str);
    this->format = format;
    is_repeat_code = false;
    _bit_itr = 0;
}

int IRData::setDataByString(String data_hex_str)
{
    unsigned long len = data_hex_str.length();
    if(len <= 0)
    {
        return -1;
    }

    clear();
    for(auto i=0; i<len; i++)
    {
        appendByHexChar(data_hex_str[i]);
    }
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
    _bit_itr = 0;
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

long IRData::appendByte(byte bytedata)
{
    long byte_idx = _data_len_bits >> 3;
    _data_len_bits &= 0xFFFFFFF8;
    if(_data_len_bits < (capacity() << 3))
    {
        _buffer[byte_idx] = bytedata;
        _data_len_bits += 8;
        return _data_len_bits;
    }
    return -1;
}

long IRData::appendHex(byte hexdata)
{
    long byte_idx = _data_len_bits >> 3;
    _data_len_bits &= 0xFFFFFFFC;

    if(_data_len_bits < (capacity() << 3))
    {
        _buffer[byte_idx] = (_buffer[byte_idx] << 4) | (hexdata & 0xF);
        _data_len_bits += 4;
        return _data_len_bits;
    }
    return -1;
}

long IRData::appendByHexChar(char hex_char)
{
    if(hex_char >= '0' && hex_char <='9')
    {
        return appendHex(byte(hex_char - '0'));
    }
    else if(hex_char >= 'a' && hex_char <= 'f')
    {
        return appendHex(byte(hex_char - 'a') + 0xA);
    }
    else if(hex_char >= 'A' && hex_char <= 'F')
    {
        return appendHex(byte(hex_char - 'A') + 0xA);
    }
    return -1;
}

int IRData::fetch()
{
    if(_bit_itr >= bitLengh())
    {
        return -1;
    }

    long byte_idx = _bit_itr >> 3;
    byte bit_idx = 7 - (_bit_itr & 0x7);

    int bit =  (_buffer[byte_idx] >> bit_idx) & 0x1;
    _bit_itr++;
    return bit;
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

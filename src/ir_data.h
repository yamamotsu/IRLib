#ifndef _IR_DATA_H_
#define _IR_DATA_H_

const int _irdata_buffer_size = 128;

enum IRDataFormat
{
    AEHA,
    NEC,
    AUTO
};

class IRData
{
public:
private:
    byte _buffer[_irdata_buffer_size];
    long _data_len_bits;
    long _bit_itr;
public:
    IRDataFormat format;
    bool is_repeat_code;

    IRData();
    void clear();
    long append(byte databit);
    int fetch();
    const int capacity();
    const int bitLengh();
    const int get(int index);
    const int operator[] (int index);
};

#endif // _IR_DATA_H_

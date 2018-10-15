#include <arduino.h>
#include "ir_led.h"

IRLed::IRLed(byte pin_led, bool invert_out)
{
    this->_pin_led = pin_led;
    if(invert_out)
    {
        _high = 0;
        _low = 1;
    }
    else
    {
        _high = 1;
        _low = 0;
    }

    pinMode(this->_pin_led, OUTPUT);
}

void IRLed::output(byte bit,
                    long howlong_us,
                    int career_high_us,
                    int career_low_us)
{
    unsigned long t0 = micros();
    if(bit)
    {
        while(micros() - t0 <= howlong_us)
        {
            digitalWrite(_pin_led, _high);
            delayMicroseconds(career_high_us);
            digitalWrite(_pin_led, _low);
            delayMicroseconds(career_low_us);
        }
    }
    else
    {
        digitalWrite(_pin_led, _low);
        delayMicroseconds(howlong_us);
    }
}

int IRLed::sendIRData(IRData &data)
{
    int unit_us;
    int leader_high_len, leader_low_len;
    int data_high_len[2], data_low_len[2];
    int career_high_us, career_low_us;
    int bit;

    byte bitline = 0, bitcount=0;

    switch(data.format)
    {
    case IRDataFormat::NEC:
        // career: 38kHz, 1/3 duty
        career_high_us = 9;
        career_low_us = 18;

        unit_us = 562;
        leader_high_len = 16;
        leader_low_len = 8;
        data_high_len[0] = 1;
        data_low_len[0] = 1;
        data_high_len[1] = 1;
        data_low_len[1] = 3;
        break;
    case IRDataFormat::AEHA:
        // career: 38kHz, 1/3 duty
        career_high_us = 9;
        career_low_us = 18;

        unit_us = 425;
        leader_high_len = 8;
        leader_low_len = 4;
        data_high_len[0] = 1;
        data_low_len[0] = 1;
        data_high_len[1] = 1;
        data_low_len[1] = 3;
        break;
    default:
        // invalid format data
        return -1;
    }

    // send leader code
    output(1, leader_high_len * unit_us, career_high_us, career_low_us);
    //delayMicroseconds(leader_high_len * unit_us);
    output(0, leader_low_len * unit_us, career_high_us, career_low_us);
    //delayMicroseconds(leader_low_len * unit_us);

    //debug
    //Serial.println("leader code sended");

    bit = data.fetch();
    while(bit >= 0)
    {
        output(1, data_high_len[bit] * unit_us, career_high_us, career_low_us);
        output(0, data_low_len[bit] * unit_us, career_high_us, career_low_us);
        bit = data.fetch();
    }

    //Serial.println("detected end of data");

    // Stop trailer
    output(1, data_high_len[1] * unit_us, career_high_us, career_low_us);
    output(0, 10000, career_high_us, career_low_us);
    return 0;
}

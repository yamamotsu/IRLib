#include <arduino.h>
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

    _status = IRStatus::stop;

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

int IRReceiver::_waitSignal(long timeout_ms)
{
    unsigned long t0 = millis();
    while(input() == 0)
        if(millis() - t0 > timeout_ms && timeout_ms > 0)
            return -1;

    return millis() - t0;
}

int IRReceiver::_scanLeaderCode(IRData &data, long high_time, long low_time, bool format_autodetect)
{
    if(high_time >= 2000 && high_time < 5000)
    {
        if(low_time >= 1000 && low_time < 2500)
        {
            // AEHA Leader Code detected.
            _status = IRStatus::leader_detected;
            if (format_autodetect)
            {
                data.format = IRDataFormat::AEHA;
            }
        }
        else if(low_time >= 2500 && low_time < 5000)
        {
            // AEHA Repeat Code detected.
            _status = IRStatus::repeat_detected;
            data.is_repeat_code = true;
        }
        else
        {
            return -1;
        }
    }
    else if(high_time >= 8000 && high_time < 12000)
    {
        if (low_time >= 4000 && low_time < 6000)
        {
            // NEC Leader Code detected.
            _status = IRStatus::leader_detected;
            if (format_autodetect)
            {
                data.format = IRDataFormat::NEC;
            }
        }
        else if(low_time >= 2000 && low_time < 3000)
        {
            // NEC Repeat Code detected.
            _status = IRStatus::repeat_detected;
            data.is_repeat_code = true;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int IRReceiver::_getDataBit(long high_time, long low_time)
{
    // Hardcoding: current available format:AEHA & NEC
    if(high_time >= 350 && high_time < 800)
    {
        if(low_time >= 350 && low_time < 800)
        {
            // '0' received
            return 0;
        }
        else if(low_time >= 1000 && low_time < 2000)
        {
            // '1' received
            return 1;
        }
    }
    // invalid data code error
    return -1;
}

int IRReceiver::getIRData(IRData &data, long timeout_ms)
{
    byte signal;
    long high_time = 0,low_time = 0;
    unsigned long t0, t1;
    bool leader_detected = false;
    bool stop_detected = false;
    int databit;

    data.clear();

    // wait for signal
    if(_waitSignal(timeout_ms) == -1)
    {
        // timeout return
        return -1;
    }
    _status = IRStatus::signal_detected;

    t0 = micros();
    while(1)
    {
        signal = input();
        if(signal == 0)
        {
            if(low_time > 8000)
            {
                // Stop bit detect
                if(_status == IRStatus::signal_detected)
                {
                    return -1;
                }

                _status = IRStatus::stop;
                break;
            }
            t1 = micros();
            low_time += t1 - t0;
            t0 = t1;
        }
        else
        {
            if(low_time > 0)
            {

                switch (_status)
                {
                    case IRStatus::signal_detected:

                        if(_scanLeaderCode(data, high_time, low_time) == -1)
                        {
                            // invalid leader code
                            return -1;
                        }

                        break;
                    case IRStatus::leader_detected:
                        databit = _getDataBit(high_time, low_time);
                        if(databit >= 0) data.append(databit);
                        break;
                    case IRStatus::repeat_detected:
                        break;
                }
                high_time = 0;
                low_time = 0;
            }
            t1 = micros();
            high_time += t1 - t0;
            t0 = t1;
        }
    }

    return 0;
}

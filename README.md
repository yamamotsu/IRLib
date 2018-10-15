# IRLib
Simple IR ray signal I/O library for Arduino.

## Description
Arduinoに接続した赤外線LEDから任意のデータを出力したり,赤外線受信モジュールからの信号読み取りを簡単に行えるライブラリです.  

## Install
```
git clone https://github.com/yamamotsu/IRLib.git
cp ./IRLib ~/Documents/Arduino/libraries/
```

## Demo

### 赤外線を受信するサンプルコード

```
#include <ir_lib.h>         //include library

int pin_receiver = 4;       // receiver pin number
bool invert_in = true;      // invert input

IRReceiver receiver(pin_receiver, invert_in);

// create IRData class instance to store received data.
IRData data;

void setup()
{
    Serial.begin(115200);

    long timeout_ms = -1;
    // get IR code and store into IRData instance
    receiver.getIRData(data, timeout_ms);

    long len = data.bitLengh() >> 3;
    for(auto i=0; i < len; i++)
    {
        // print received data byte on display
        Serial.print(data[i], HEX);
    }
    Serial.println("");
}
void loop()
{

}
```

### 赤外線を送信するサンプルコード
```
#include <ir_lib.h>         //include library

int pin_led = 5;            // IR led pin number
bool invert_out = true;     // invert output

IRLed led(pin_led, invert_out);

void setup()
{
    Serial.begin(115200);

    // create IRData instance and store data to send.
    IRData data(String("ABCDEF12340"), IRDataFormat::NEC);

    led.sendIRData(data);
}
void loop()
{

}
```

## Usage

赤外線を読み取る場合でも出力する場合でも,最初にライブラリのインクルードが必要です.
```
#include <ir_lib.h>
```
赤外線受信モジュールを接続したピンから信号を読み取るには,
まず`IRReceiver`クラスのインスタンスを作成します.
```
int  pin_receiver = 4;
bool invert_in = true;
IRReceiver receiver(pin_receiver, invert_in);
```
コンストラクタ, invert_inの引数`pin_receiver`には赤外線受信モジュールを接続しているArduinoピン番号を指定します.  
`invert_in`には負論理入力の場合`true`を,そうでない場合は`false`を指定します.`invert_in`を指定しない場合デフォルト値として`false`が使用されます.  
次に受信した赤外線データを格納するための`IRData`クラスのインスタンスを作成します.
```
IRData data;
```

## Licence

[MIT](https://github.com/yamamotsu/IRLib/blob/master/LICENSE)

## Author

[yamamotsu](https://github.com/yamamotsu)

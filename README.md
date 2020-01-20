
RF430CL330H_Shield (DNFC Tag)
==================

The DNFC (dynamic NFC) tag is reprogrammable with a microcontroller with an I2C interface thanks to TI's RF430CL330H dynamic NFC transponder IC.

## Info
- Indiegogo - [info](https://www.indiegogo.com/projects/dnfc-tag-nfc-that-interacts-with-microcontroller)

![nfc tag](http://www.elecfreaks.com/store/images/DNFC%20TAG-01.jpg)

## Connect Guide

```
//                  DNFC Tag                   (Host/Tester)
//                 RF430CL330H                Arduino 2560/UNO
//                  (Slave)                      Master
//             _________________            _________________
//  5V/3.3V<--|VCC           SDA|<-------->|P20(2560)/A4(UNO)|
//            |                 |    I2C   |                 |
//            |              SCL|<-------->|P21(2560)/A5(UNO)|
//            |                 |          |                 |
//      GND<--|GND          /RST|<---------|P4               |
//            |             INTO|--------->|P3(INT1)         |
//            |                 |          |                 |
//            |                 |          |                 |
//            |                 |          |                 |
//            |_________________|          |_________________|
```

## Start up:
- connect Arduino 2560/UNO with NFC Tag
- make this library to arduino IDE
- use example to test your Tag

## Easy use with NDEF
You can write a NDEF message, like this:
```
NdefRecord records[1];
records[0].createUri("https://github.com");
```
Or more than one:
```
NdefRecord records[2];
records[0].createUri("https://github.com");
records[1].createUri("https://google.com");
```

We can write data in different formats for example, URI, External, MIME, Text, even Application what can ope app directly. See example/createNDEFmessage.ino

## NFC flower demo
use NFC to water flower, and take the temperature, humidity, soil moisture.
- Github Arduino Code - [water system](https://github.com/nfcwormhole/NFC-Flower/tree/master/Arduino/nfc_flower)
- Github Android App Code - [water system](https://github.com/nfcwormhole/NFC-Flower/tree/master/Android/NFC_Flower)
- Google Play download- [water system](https://play.google.com/store/apps/details?id=com.flower.nfcaction)

[![Analytics](https://ga-beacon.appspot.com/UA-61411142-2/RF430CL330H_Shield)](https://github.com/igrigorik/ga-beacon)

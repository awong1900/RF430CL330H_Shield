
DNFC Tag: NFC that Interacts with Microcontroller
==================
The DNFC (Dynamic Near Field Communication) tag, open-source NFC tag that communicates with microcontroller.

## Info
- [Indiegogo successful crowdfunding](https://www.indiegogo.com/projects/dnfc-tag-nfc-that-interacts-with-microcontroller)

![nfc tag](http://www.elecfreaks.com/store/images/DNFC%20TAG-01.jpg)

## Connect Guide

```
//                  DNFC Tag                   (Host/Tester)
//                 RF430CL330H                Arduino 2560/UNO
//                  (Slave)                      Master
//             _________________            _________________
//     3.3V<--|VCC           SDA|<-------->|P20(2560)/A4(UNO)|
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
1. Connect Arduino 2560/UNO with NFC Tag
1. Make this library to arduino IDE
1. Use example to write Tag
1. Use NFC Reader read it


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
records[1].createUri("https://github.com/awong1900/RF430CL330H_Shield");
```

You can write data in different formats for example, URI, External, MIME, Text, even Application what can open app directly.

## Recommend Reader App
Android App [NFC Taginfo by NXP](https://play.google.com/store/apps/details?id=com.nxp.taginfolite)

[![Analytics](https://ga-beacon.appspot.com/UA-61411142-2/RF430CL330H_Shield)](https://github.com/igrigorik/ga-beacon)

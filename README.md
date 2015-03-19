
RF430CL330H_Shield (DNFC Tag)
==================
## [Indiegogo](https://www.indiegogo.com/projects/dnfc-tag-nfc-that-interacts-with-microcontroller)
More info : http://www.elecfreaks.com

The DNFC tag is reprogrammable with a microcontroller with an I2C interface thanks to TI's RF430CL330H dynamic NFC transponder IC.

## Connect Guide

```
//                                /|\  /|\    (Host/Tester)
//                  DNFC Tag      10k  10k     Arduino 2560/UNO
//                  (Slave)        |    |        Master
//             _________________   |    |   _________________
//     3.3V<--|VCC           SDA|<-|----+->|P20/A4(UNO)      |
//            |                 |  | I2C   |                 |
//            |              SCL|<-+------>|P21/A5(UNO)      |
//            |                 |          |                 |
//      GND<--|GND          /RST|<---------|P4               |
//            |             INTO|--------->|P3(INT1)         |
//            |                 |          |                 |
//            |                 |          |                 |
//            |                 |          |                 |
//            |_________________|          |_________________|
```

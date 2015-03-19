
RF430CL330H_Shield (DNFC Tag)
==================
Buy DNFC Tag here http://igg.me/at/GOODNFCTAG/x/7649823
More info : http://www.elecfreaks.com

The DNFC tag is reprogrammable with a microcontroller with an I2C interface thanks to TI's RF430CL330H dynamic NFC transponder IC.

Connect Guide
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

RF430 User Address Map
//  -----------------------------------------
//  Address     | Size  | Description       |
//  -----------------------------------------
//  0xFFFE      | 2B    | Control Register  |
//  0xFFFC      | 2B    | Status Register   |
//  0xFFFA      | 2B    | Interrupt Enable  |
//  0xFFF8      | 2B    | Interrupt Flags   |
//  0xFFF6      | 2B    | CRC Result        |
//  0xFFF4      | 2B    | CRC Length        |
//  0xFFF2      | 2B    | CRC Start Address |
//  0xFFF0      | 2B    | Comm WD Ctrl Reg  |
//  -----------------------------------------
//  0x0000 -    | 2kB   | NDEF App Memory   |
//  0x07FF      |       |                   |

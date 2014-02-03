
RF430CL330H_Shield
==================

The Texas Instruments Dynamic NFC Interface Transponder RF430CL330H is a NFC Tag Type 4 device.

/*****************************************
IMPORTANT:
change arduino wire library about I2C buffer.
1.arduino-1.0.5\libraries\Wire\utility\twi.h
#define TWI_BUFFER_LENGTH 255
2.arduino-1.0.5\libraries\Wire\Wire.h
#define BUFFER_LENGTH 255
*******************************************/

Connect Guide
//                                /|\  /|\    (Host/Tester)
//                   RF430        10k  10k     Arduino 2560
//                  (Slave)        |    |        Master
//             _________________   |    |   _________________
//            |              SDA|<-|----+->|P20              |
//            |                 |  | I2C   |                 |
//            |              SCL|<-+------>|P21              |
//            |                 |          |                 |
//      GND<--|E(2-0)       /RST|<---------|P4               |
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

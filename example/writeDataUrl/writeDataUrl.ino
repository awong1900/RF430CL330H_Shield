/* Copyright 2013-2014 Ten Wong, wangtengoo7@gmail.com
*  https://github.com/awong1900/RF430CL330H_Shield
*/

/*********************************************************
** sample: when reset the rf430, it will write the uri to
** rf430 tag.
***********************************************************/
#include <RF430CL330H_Shield.h>

//for UNO, Nona...
#define IRQ   (1)   //External interrupt 3
#define RESET (4)

//for Elecfreak NFC TAG BOT V1.2(base on Leonardo)
//#define IRQ   (4)  //External interrupt 7
//#define RESET (6)

int led = 13;
RF430CL330H_Shield nfc(IRQ, RESET);

/* https://github.com/awong1900/RF430CL330H_Shield  */
#define RF430_URI_DATA {    \
    /*NDEF Tag Application */    \
    0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,         \
            \
    0xE1, 0x03,     /*Capability Container ID*/     \
            \
    /* CC file start */    \
    0x00, 0x0F,     /* CCLEN 15bytes fix*/    \
    0x20,     /* Mapping version 2.0 */    \
    0x00, 0xF9,     /* MLe (249 bytes); Maximum R-APDU data size */    \
    0x00, 0xF6,     /* MLc (246 bytes); Maximum C-APDU data size */    \
    0x04,     /* Tag, File Control TLV (4 = NDEF file) */    \
    0x06,     /* Length, File Control TLV (6 = 6 bytes of data for this tag) */    \
    0xE1, 0x04,     /* Type4 Tag File Identifier */    \
    0x0B, 0xDF,     /* Max NDEF size (3037 bytes of RF430CL330 useable memory) */    \
    0x00,     /* NDEF file read access condition, read access without any security */    \
    0x00,     /* NDEF file write access condition; write access without any security */    \
    /* CC file end */    \
            \
    0xE1, 0x04,     /* NDEF File ID */     \
            \
    0x00, 0x2C,    /* NDEF Length 44bytes */    \
            \
    /* NDEF start */    \
    0xD1,    /* NDEF Header MB=1, ME=1, CF=0, SR=1, IL=0, TNF=1 */    \
            \
    0x01,    /* Type Length 1 byte */    \
            \
    0x28,    /* Payload length 40bytes */    \
            \
    0x55,    /* Type U (URI) */    \
    /* Payload start */    \
            \
    0x04,     /* URI Record Type : htps://  */    \
    /*Appliction Data : github.com/awong1900/RF430CL330H_Shield */    \
    0x67, 0x69, 0x74, 0x68, 0x75, 0x62, 0x2E, 0x63,         \
    0x6F, 0x6D, 0x2F, 0x61, 0x77, 0x6F, 0x6E, 0x67,         \
    0x31, 0x39, 0x30, 0x30, 0x2F, 0x52, 0x46, 0x34,         \
    0x33, 0x30, 0x43, 0x4C, 0x33, 0x33, 0x30, 0x48,         \
    0x5F, 0x53, 0x68, 0x69, 0x65, 0x6C, 0x64,         \
} /* End of data */

unsigned char NDEF_Application_Data[] = RF430_URI_DATA;

volatile byte into_fired = 0;
uint16_t flags = 0;

void setup(void)
{
    Serial.begin(115200);
    Serial.println("Hello!");
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    //RF430 init
    nfc.begin();

    //write NDEF memory with Capability Container + NDEF message
    nfc.Write_Continuous(0, NDEF_Application_Data, sizeof(NDEF_Application_Data));

    //set NDEF message read-only
    //nfc.SetReadOnly(true);

    //enable interrupt
    attachInterrupt(IRQ, RF430_Interrupt, FALLING);

    Serial.println("Wait for read or write...");
}

void loop(void)
{
    if(into_fired)
    {
        //clear control reg to disable RF
        nfc.Write_Register(CONTROL_REG, nfc.Read_Register(CONTROL_REG) & ~RF_ENABLE);
        delay(750);

        //read the flag register to check if a read or write occurred
        flags = nfc.Read_Register(INT_FLAG_REG);
        //Serial.print("INT_FLAG_REG = 0x");Serial.println(flags, HEX);

        //ACK the flags to clear
        nfc.Write_Register(INT_FLAG_REG, EOW_INT_FLAG + EOR_INT_FLAG);

        if(flags & EOW_INT_FLAG)      //check if the tag was written
        {
            Serial.println("The tag was written!");
            digitalWrite(led, LOW);
            delay(2000);
            digitalWrite(led, HIGH);
        }
        else if(flags & EOR_INT_FLAG) //check if the tag was read
        {
            Serial.println("The tag was readed!");
            digitalWrite(led, LOW);
            delay(1000);
            digitalWrite(led, HIGH);
        }

        flags = 0;
        into_fired = 0; //we have serviced INT1

        //Configure INTO pin for active low and re-enable RF
        nfc.Write_Register(CONTROL_REG, nfc.Read_Register(CONTROL_REG) | RF_ENABLE);

        //re-enable INTO
        attachInterrupt(IRQ, RF430_Interrupt, FALLING);
    }

    delay(100);
}

/**
**  @brief  interrupt service
**/
void RF430_Interrupt()
{
    into_fired = 1;
    detachInterrupt(IRQ);//cancel interrupt
}

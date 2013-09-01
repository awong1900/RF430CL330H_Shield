/* Copyright 2013 Ten Wong, wangtengoo7@gmail.com  
*  https://github.com/awong1900/RF430CL330H_Shield 
*  RF430CL330H datasheet reference http://www.ti.com/
*/

/*********************************************************
** sample: when reset the rf430, it will write the uri to 
** rf430 tag.
***********************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>
#include <RF430CL330H_Shield.h>

#define IRQ   (3)
#define RESET (4)  
int led = 13;
RF430CL330H_Shield nfc(IRQ, RESET);

/* http://www.time.com/time/ */
#define RF430_TIME_DATA {    \
    /*NDEF Tag Application */    \
    0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,         \
            \
    0xE1, 0x03,     /*Capability Container ID*/     \
            \
    /* CC file start */    \
    0x00, 0x0F,     /* CCLEN 15bytes fix*/    \
    0x20,     /* Mapping version 2.0 */    \
    0x00, 0x3B,     /* MLe (59 bytes); Maximum R-APDU data size */    \
    0x00, 0x34,     /* MLc (52 bytes); Maximum C-APDU data size */    \
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
    0x00, 0x13,    /* NDEF Length 19bytes */    \
            \
    /* NDEF start */    \
    0xD1,    /* NDEF Header MB=1, ME=1, CF=0, SR=1, IL=0, TNF=1 */    \
            \
    0x01,    /* Type Length 1 byte */    \
            \
    0x0F,    /* Payload length 15bytes */    \
            \
    0x55,    /* Type U (URI) */    \
    /* Payload start */    \
            \
    0x01,     /* URI Record Type : http://www.  */    \
    /*Appliction Data : time.com/time/ */    \
    0x74, 0x69, 0x6D, 0x65, 0x2E, 0x63, 0x6F, 0x6D,         \
    0x2F, 0x74, 0x69, 0x6D, 0x65, 0x2F,         \
            \
            \
            \
            \
            \
            \
            \
            \
            \
            \
            \
            \
            \
            \
} /* End of data */
unsigned char NDEF_Application_Data[] = RF430_TIME_DATA;

volatile byte into_fired = 0;
uint16_t flags = 0;

void setup(void) 
{
    Serial.begin(115200);    
    Serial.println("Hello!");
    pinMode(led, OUTPUT); 
    digitalWrite(led, HIGH);
    //reset RF430
    nfc.begin();
    delay(1000);
}

void loop(void) 
{
    while(!(nfc.Read_Register(STATUS_REG) & READY)); //wait until READY bit has been set
    Serial.print("Fireware Version:"); Serial.println(nfc.Read_Register(VERSION_REG), HEX);    

    //write NDEF memory with Capability Container + NDEF message
    nfc.Write_Continuous(0, NDEF_Application_Data, sizeof(NDEF_Application_Data));

    //Enable interrupts for End of Read and End of Write
    nfc.Write_Register(INT_ENABLE_REG, EOW_INT_ENABLE + EOR_INT_ENABLE);

    //Configure INTO pin for active low and enable RF
    nfc.Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE );

    //enable interrupt 1
    attachInterrupt(1, RF430_Interrupt, FALLING);
    
    Serial.println("Wait for read or write...");
    while(1)
    {
        if(into_fired)
        {
            //clear control reg to disable RF
            nfc.Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE); 
            delay(750);
            
            //read the flag register to check if a read or write occurred
            flags = nfc.Read_Register(INT_FLAG_REG); 
            Serial.print("INT_FLAG_REG = 0x");Serial.println(flags, HEX);
            
            //ACK the flags to clear
            nfc.Write_Register(INT_FLAG_REG, EOW_INT_FLAG + EOR_INT_FLAG); 
            
            if(flags & EOW_INT_FLAG)      //check if the tag was written
            {
                Serial.println("The tag was writted!");
                digitalWrite(led, HIGH);
            }
            else if(flags & EOR_INT_FLAG) //check if the tag was read
            {
                Serial.println("The tag was readed!");
                digitalWrite(led, LOW);
            }
            flags = 0;
            into_fired = 0; //we have serviced INT1

            //Enable interrupts for End of Read and End of Write
            nfc.Write_Register(INT_ENABLE_REG, EOW_INT_ENABLE + EOR_INT_ENABLE);

            //Configure INTO pin for active low and re-enable RF
            nfc.Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE);

            //re-enable INTO
            attachInterrupt(1, RF430_Interrupt, FALLING);
        }
    }
}

/**
**  @brief  interrupt service
**/
void RF430_Interrupt()            
{
    into_fired = 1;
    detachInterrupt(1);//cancel interrupt
}


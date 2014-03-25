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
#include <NdefMessage.h>
#include <NdefRecord.h>

#define IRQ   (3)
#define RESET (4)  
RF430CL330H_Shield nfc(IRQ, RESET);

volatile byte into_fired = 0;
uint16_t flags = 0;
int i =0;
void setup(void) 
{
    Serial.begin(115200);    
    Serial.println("Hello!");

    //reset RF430
    nfc.begin();
    
    //enable interrupt 1
    attachInterrupt(1, RF430_Interrupt, FALLING);
        
    Serial.println("Wait for read or write...");
}

void loop(void) 
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
            Serial.println("The tag was written!");
        }
        else if(flags & EOR_INT_FLAG) //check if the tag was read
        {
            Serial.println("The tag was readed!");
        }
        flags = 0;
        into_fired = 0; //we have serviced INT1
        //Configure INTO pin for active low and re-enable RF
        nfc.Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE);
        //re-enable INTO
        attachInterrupt(1, RF430_Interrupt, FALLING);
    }

    //modify NDEF message via srial interface
    NdefRecord records[2];
    records[0].createUri("http://www.time.com");
    byte exData[] = {0x10}; //humidity data
    String domain = "flower"; 
    String type = "humidity";
    records[1].createExternal(domain, type, exData, sizeof(exData));
    if (i%10 == 1)
    {
        NdefMessage msg(records, sizeof(records)/sizeof(NdefRecord));
        uint16_t msg_length = msg.getByteArrayLength();
        byte message[msg_length];
        msg.toByteArray(message);
        nfc.Write_NDEFmessage(message, msg_length);

#if 1 //wteng 2014-3-25
        byte buffer[1024];
        nfc.Read_Continuous(0, buffer, 200);
        Serial.println("buffer[] = ");
        for (uint8_t i=0; i<200; i++)
        {
            Serial.print(buffer[i], HEX);
            Serial.print(" ");
            if (i%0x10 == 0x0F)
                Serial.println();  
        }
        Serial.println();  
#endif
        i++;
    }
    else
        i++;
    
    Serial.print("loop = ");Serial.println(i);
    delay(10000);
}

/**
**  @brief  interrupt service
**/
void RF430_Interrupt()            
{
    into_fired = 1;
    detachInterrupt(1);//cancel interrupt
}


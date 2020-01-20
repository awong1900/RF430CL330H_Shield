/* Copyright 2013-2020 Ten Wong, wangtengoo7@gmail.com
*  https://github.com/awong1900/RF430CL330H_Shield
*/

/*******************************************************************
** sample: easy to creat various NDEF format records,
** such as URI, MIME, Text, External, Application and so on.
*******************************************************************/
#include <RF430CL330H_Shield.h>
#include <NdefMessage.h>
#include <NdefRecord.h>

//for UNO, Nona...
#define IRQ   (1)   //External interrupt 3
#define RESET (4)

//for Elecfreak NFC TAG BOT V1.2(base on Leonardo)
//#define IRQ   (4)  //External interrupt 7
//#define RESET (6)

int led = 13;
RF430CL330H_Shield nfc(IRQ, RESET);

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

/** easy to create NDEF message with library NDEFRecord and NDEFMessage **/

    NdefRecord records[5];

    //#record0 for URI
    records[0].createUri("https://github.com/awong1900/RF430CL330H_Shield");

    //#record1 for External
    byte exData[] = {0x10, 0x11, 0x12, 0x13, 0x14};
    String domain = "nfc";
    String type = "dtag";
    records[1].createExternal(domain, type, exData, sizeof(exData));

    //#record2 for MIME
    String mimeType = "application/vnd.com.example.android.beam";
    byte mimeData[] = "Beam me up, Android";
    records[2].createMime(type, exData, sizeof(exData));

    //#record3 for Text
    byte text_encode[] = "Hey, Genius!";
    records[3].createText(text_encode, sizeof(text_encode), ENGLISH, true);

    //#record4 for Application
    String packageName = "com.google.android.apps.maps";
    records[4].createApplicationRecord(packageName);

    //define ndef message
    NdefMessage msg(records, sizeof(records)/sizeof(NdefRecord));
    uint16_t msg_length = msg.getByteArrayLength();
    byte message[msg_length];

    //get ndef message
    msg.toByteArray(message);

    //write message to TAG memory
    nfc.Write_NDEFmessage(message, msg_length);

/** write message end **/

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

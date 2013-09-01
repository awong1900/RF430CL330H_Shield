/* Copyright 2013 Ten Wong, wangtengoo7@gmail.com  
*  https://github.com/awong1900/RF430CL330H_Shield 
*/
//this code is heavily borrowed from Adafruit_NFCShield_I2C
//link to original https://github.com/adafruit/Adafruit_NFCShield_I2C

#include "RF430CL330H_Shield.h"

/* Uncomment these lines to enable debug output for RF430(I2C) */
//#define RF430DEBUG

 
/**
**  @brief  Sends a single byte via I2C
**  @param  x    The byte to send
**/
static inline void wiresend(uint8_t x) 
{
  #if ARDUINO >= 100
    Wire.write((uint8_t)x);
  #else
    Wire.send(x);
  #endif
}


/**
**  @brief  Reads a single byte via I2C
**/
static inline uint8_t wirerecv(void) 
{
  #if ARDUINO >= 100
    return Wire.read();
  #else
    return Wire.receive();
  #endif
}


/**
**  @brief  Instantiates a new RF430 class
**  @param  irq       Location of the IRQ pin
**  @param  reset     Location of the RSTPD_N pin
**/
RF430CL330H_Shield::RF430CL330H_Shield(uint8_t irq, uint8_t reset)
{
    byte RxData[2] = {0,0};
    byte TxData[2] = {0,0};
    byte TxAddr[2] = {0,0};

    //_irq = irq;
    _reset = reset;

    //pinMode(_irq, INPUT); //other shield do not use this shield
    pinMode(_reset, OUTPUT);
}


/** 
**  @brief  Setups the HW 
**/
void RF430CL330H_Shield::begin()
{
    Wire.begin();
    // Reset the RF430  
    digitalWrite(_reset, HIGH);
    digitalWrite(_reset, LOW);
    delay(100);                   //Reset:low level 100ms
    digitalWrite(_reset, HIGH);
}


/** 
**  @brief  Reads the register at reg_addr, returns the result
**  @param  uint16_t    reg_addr    RF430 Register address
**  @retrun uint16_t    the value of register
**/
uint16_t RF430CL330H_Shield::Read_Register(uint16_t reg_addr)
{
    TxAddr[0] = reg_addr >> 8;      // MSB of address
    TxAddr[1] = reg_addr & 0xFF;    // LSB of address TxAddr[0]TxAddr[1]
#ifdef RF430DEBUG    
    Serial.print("Read_Register[0x");Serial.print(reg_addr, HEX);Serial.print("]:0x");
#endif
    //send slave addr
    Wire.beginTransmission(RF430_I2C_ADDRESS);
    wiresend(TxAddr[0]); //bit15~8
    delay(1);
    wiresend(TxAddr[1]); //bit7~0
    Wire.endTransmission(); 
    
    //resend slave addr with data
    Wire.requestFrom((uint8_t)RF430_I2C_ADDRESS, (uint16_t)2);
    RxData[0] = wirerecv();
    //delay(1);
    RxData[1] = wirerecv();
    
    //send stop
    Wire.endTransmission();
    
#ifdef RF430DEBUG    
    Serial.println(RxData[1] << 8 | RxData[0], HEX);
#endif
    return RxData[1] << 8 | RxData[0];
}


/** 
**  @brief  Continuous read data_length bytes and store in the area "read_data"
**  @param  uint16_t    reg_addr       RF430 Register address
**  @param  uint8_t*    read_data      buffer for store the data
**  @param  uint16_t    data_length    length of data    
**  @retrun void
**/
void RF430CL330H_Shield::Read_Continuous(uint16_t reg_addr, uint8_t* read_data, uint16_t data_length)
{
    uint16_t i;

    TxAddr[0] = reg_addr >> 8;      // MSB of address
    TxAddr[1] = reg_addr & 0xFF;    // LSB of address

    //send slave addr
    Wire.beginTransmission(RF430_I2C_ADDRESS);
    wiresend(TxAddr[0]); //bit15~8
    wiresend(TxAddr[1]); //bit7~0
    Wire.endTransmission();
    
    //resend slave addr with data
    Wire.requestFrom((uint8_t)RF430_I2C_ADDRESS, data_length);
    //wirerecv();
    while(Wire.available())
    {
        for (uint8_t i=0; i<data_length; i++) 
        {
            read_data[i] = wirerecv();
            //delay(1);
        }
    }
    
    //send stop
    Wire.endTransmission();
#ifdef RF430DEBUG    
    Serial.print("RxData[] = 0x");
    for (uint8_t i=0; i<data_length; i++) 
    {
        Serial.print(read_data[i], HEX); 
    }
    Serial.println("");
#endif
}


/** 
**  @brief  writes the register at reg_addr with value
**  @param  uint16_t    reg_addr    RF430 Register address
**  @param  uint16_t    value       writted value   
**  @retrun void
**/
void RF430CL330H_Shield::Write_Register(uint16_t reg_addr, uint16_t value)
{
    TxAddr[0] = reg_addr >> 8;      // MSB of address
    TxAddr[1] = reg_addr & 0xFF;    // LSB of address
    TxData[0] = value >> 8;
    TxData[1] = value & 0xFF;
#ifdef RF430DEBUG    
        Serial.print("Write_Register[0x");Serial.print(reg_addr, HEX);
        Serial.print("]:0x");Serial.println(value, HEX);
#endif
    //send slave addr
    Wire.beginTransmission(RF430_I2C_ADDRESS);
    wiresend(TxAddr[0]); //bit15~8
    delay(1);
    wiresend(TxAddr[1]); //bit7~0

    //send value
    wiresend(TxData[1]); //bit7~0
    delay(1);
    wiresend(TxData[0]); //bit15~8

    //send stop
    Wire.endTransmission();
    
}


/** 
**  @brief  writes the register at reg_addr and incrementing addresses with the data at "write_data" of length data_length
**  @param  uint16_t    reg_addr       RF430 Register address
**  @param  uint8_t*    write_data     buffer for store the data
**  @param  uint16_t    data_length    length of data    
**  @retrun void
**/
void RF430CL330H_Shield::Write_Continuous(uint16_t reg_addr, uint8_t* write_data, uint16_t data_length)
{
    uint16_t i;
#ifdef RF430DEBUG    
    Serial.print("reg_addr = 0x");Serial.println(reg_addr, HEX);
    Serial.print("data_length = 0x");Serial.println(data_length, HEX);
    Serial.print("write_data[] = ");
    for (i=0; i<data_length; i++)
        Serial.print(write_data[i], HEX);
    Serial.println();  
#endif
    TxAddr[0] = reg_addr >> 8;      // MSB of address
    TxAddr[1] = reg_addr & 0xFF;        // LSB of address

    //send slave addr
    Wire.beginTransmission(RF430_I2C_ADDRESS);
    wiresend(TxAddr[0]); //bit15~8
    wiresend(TxAddr[1]); //bit7~0
    
    //send data
    for (uint8_t i=0; i<data_length; i++) 
    {
        wiresend(write_data[i]); 
        delay(1);
    }
    //send stop
    Wire.endTransmission();
}



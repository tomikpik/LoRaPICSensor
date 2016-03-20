#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

#include "soft_i2c.h"


#define _XTAL_FREQ 64000000

void I2C_SDA_SET()   { LATC4 = 1; }
void I2C_SDA_CLEAR() { LATC4 = 0; }

void I2C_SDA_IN_SET()   { TRISC4 = 1; }
void I2C_SDA_OUT_SET() { TRISC4 = 0; }

void I2C_SCL_SET()   { LATC3 = 1; }
void I2C_SCL_CLEAR() { LATC3 = 0; }

void I2C_SCL_OUT_SET() { TRISC3 = 0; }

char I2C_SDA_READ() { return PORTCbits.RC4; }


void I2C_Init()
{
    I2C_SDA_OUT_SET();
    I2C_SCL_OUT_SET();
    I2C_SCL_SET();
    I2C_SDA_SET();
    __delay_ms(10);
}

void I2C_Start()
{
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

    I2C_SDA_SET();
    I2C_SCL_SET();
    __delay_us(20);


    I2C_SDA_CLEAR();
    __delay_us(20);
    I2C_SCL_CLEAR();
    __delay_us(20);
}

void I2C_Stop()
{
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

    I2C_SCL_SET();
    __delay_us(20);
    I2C_SDA_SET();
    __delay_us(20);
}

void I2C_Write_Byte(char byt)
{
    char i;
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();
    for(i=0;i<8;i++)
    {
        if(byt & 0x80)
        {
          I2C_SDA_SET();
          __delay_us(20);
        }
        else
        {
          I2C_SDA_CLEAR();
          __delay_us(20);
        }
 
        I2C_SCL_SET();
        __delay_us(20);
        I2C_SCL_CLEAR();
        __delay_us(20);
        byt<<=1;
    }
        I2C_SDA_IN_SET();
        I2C_SCL_SET();
        __delay_us(20);
        I2C_SCL_CLEAR();
        __delay_us(20);

    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

}



char I2C_Read_Byte(char ack)
{
    char i;
    char val = 0;
    I2C_SDA_IN_SET();
    //I2C_SCL_OUT_SET();
    /*
    for(i=0;i<8;i++)
    {
        val>>=1;
        I2C_SCL_SET();
        __delay_us(20);
        if(I2C_SDA_READ()!=0)
        {
            val |= 0x80;
        }
        I2C_SCL_CLEAR();
        __delay_us(20);
    }*/
    for(i=0;i<8;i++)
    {
        if(i!=0) { val>>=1; }
        I2C_SCL_SET();
        __delay_us(4);
        if(I2C_SDA_READ()!=0)
        {
            val |= 0x80;
        }
        else
        {
            val &= 0x7F;
   
        }
        __delay_us(10);
        I2C_SCL_CLEAR();
        __delay_us(20);
    }
    
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

        if(ack)
        {
          I2C_SDA_CLEAR(); 
          I2C_SCL_SET();
          __delay_us(20);
          I2C_SCL_CLEAR();
          __delay_us(20);
        }
        else
        {
          I2C_SDA_SET(); 
          I2C_SCL_SET();
          __delay_us(20);
          I2C_SCL_CLEAR();
          __delay_us(20);
        }
   
    return val;

}

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

#include "soft_i2c.h"


#define _XTAL_FREQ 64000000

/**
 * Set SDA
 */
void I2C_SDA_SET() {
    LATC4 = 1;
}

/**
 * Clear SDA
 */
void I2C_SDA_CLEAR() {
    LATC4 = 0;
}

/**
 * Set SDA IN
 */
void I2C_SDA_IN_SET() {
    TRISC4 = 1;
}

/**
 * Set SDA OUT
 */
void I2C_SDA_OUT_SET() {
    TRISC4 = 0;
}

/**
 * Set SCL
 */
void I2C_SCL_SET() {
    LATC3 = 1;
}

/**
 * Cleat SCL
 */
void I2C_SCL_CLEAR() {
    LATC3 = 0;
}

/**
 * Set SCL OUT
 */
void I2C_SCL_OUT_SET() {
    TRISC3 = 0;
}

/**
 * Reat SDA value
 * @return SDA value
 */
char I2C_SDA_READ() {
    return PORTCbits.RC4;
}

/**
 * Soft I2C library initialization
 */
void I2C_Init() {
    I2C_SDA_OUT_SET();
    I2C_SCL_OUT_SET();
    I2C_SCL_SET();
    I2C_SDA_SET();
    __delay_ms(10);
}

/**
 * I2C start condition
 */
void I2C_Start() {
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

/**
 * I2C stop condition
 */
void I2C_Stop() {
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

    I2C_SCL_SET();
    __delay_us(20);
    I2C_SDA_SET();
    __delay_us(20);
}

/**
 * I2C write given byte 
 * @param byt
 */
void I2C_Write_Byte(char byt) {
    char i;
    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();
    for (i = 0; i < 8; i++) {
        if (byt & 0x80) {
            I2C_SDA_SET();
            __delay_us(20);
        } else {
            I2C_SDA_CLEAR();
            __delay_us(20);
        }

        I2C_SCL_SET();
        __delay_us(20);
        I2C_SCL_CLEAR();
        __delay_us(20);
        byt <<= 1;
    }
    I2C_SDA_IN_SET();
    I2C_SCL_SET();
    __delay_us(20);
    I2C_SCL_CLEAR();
    __delay_us(20);

    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

}

/**
 * I2C read byte
 * @param ack
 * @return 
 */
char I2C_Read_Byte(char ack) {
    char i;
    char val = 0;
    I2C_SDA_IN_SET();
    //I2C_SCL_OUT_SET();

    for (i = 0; i < 8; i++) {
        if (i != 0) {
            val <<= 1;
        }
        I2C_SCL_SET();
        __delay_us(20);
        if (I2C_SDA_READ() != 0) {
            val |= 0x01;
        } else {
            val &= 0xFE;

        }
        __delay_us(10);
        I2C_SCL_CLEAR();
        __delay_us(20);
    }

    I2C_SDA_OUT_SET();
    //I2C_SCL_OUT_SET();

    if (ack) {
        I2C_SDA_CLEAR();
        I2C_SCL_SET();
        __delay_us(20);
        I2C_SCL_CLEAR();
        __delay_us(20);
    } else {
        I2C_SDA_SET();
        I2C_SCL_SET();
        __delay_us(20);
        I2C_SCL_CLEAR();
        __delay_us(20);
    }

    return val;

}

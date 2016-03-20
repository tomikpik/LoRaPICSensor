#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#ifndef __I2C_H
#define __I2C_H

// Define i2c pins
#define SDA             LATC4			// Data pin for i2c
#define SDA_READ        PORTCbits.RC4			    // Data pin for i2c
#define SCK             LATC3   		// Clock pin for i2c
#define SDA_DIR         TRISC4			// Data pin direction
#define SCK_DIR         TRISC3			// Clock pin direction

// Define i2c speed
#define HalfBitDelay 4	// usec

#define _XTAL_FREQ  64000000

// Define macros
#define Set_SDA_Low		SDA_DIR = 0
#define Set_SDA_High	SDA_DIR = 1
#define Set_SCK_Low		SCK_DIR = 0
#define Set_SCK_High	SCK_DIR = 1

//Function Declarations
void InitI2C(void);
void I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
void I2C_Send_ACK(void);
void I2C_Send_NACK(void);
bit  I2C_Write_Byte(unsigned char);
unsigned char I2C_Read_Byte(void);

#endif
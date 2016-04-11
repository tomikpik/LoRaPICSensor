/*
 * File:   main.c
 * Author: Tom
 *
 * Created on 2. března 2016, 18:57
 * 
 * 
 * 
 * 1 VDD
 * 2 MCLR
 * 3 CLK
 * 4 DATA
 * 5 GND
 * 
 */

#define _XTAL_FREQ  64000000

#include <xc.h>
#include "mcc_generated_files/eusart1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/epwm1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "soft_i2c.h"

char uuid[40];
uint8_t val=0;
uint8_t inc=0;

/**
 * Waits for acknowledge from RN2483 module
 */
void wait_for_ok(void){
    while(uartState!=1){
        __delay_ms(1);
    }
    uartState=0;
}

/**
 * Delay for specified amount of time 
 * Delay time = j*5ms
 * @param j count of 5ms delays
 */
void wait_5ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(5); }   
}

/**
 * Delay for specified amount of time 
 * Delay time = j*1ms
 * @param j count of 1ms delays
 */
void wait_1ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(1); }   
}

/**
 * Requests UUID from RN2483 module and save it to variable
 */
void getUUID(){
    printf("sys get hweui\r\n");
    read_line(); 
    strcpy(uuid,line); 
}

/**
 * Setup RN2483
 * Set of RN2483 commands (look at datasheet) to set up this module.
 * This setting seems to reasonable mix of quality and message rate 
 */
void InitRadio(void){
    wait_5ms(5); 
    //resets module
    printf("sys reset\r\n");
    wait_5ms(5); 
    //read module version
    printf("sys get ver\r\n");
    read_line(); 
    //read module spread factor
    printf("radio get sf\r\n");
    read_line();  
    //read module bandwidth
    printf("radio get bw\r\n");
    read_line();  
    //read module coding rate
    printf("radio get cr\r\n");
    read_line();  
    //read module preamble length
    printf("radio get prlen\r\n");
    read_line(); 
    //read module transmit power 
    printf("radio get pwr\r\n");
    read_line(); 
    /*
     * pauses mac layer
     * This is needed because of lack of LoRa gateway. 
     * We disable MAC layer to allow modules talk to each other. 
     */
    printf("mac pause\r\n");
    read_line(); 
           
    //set power to -2
    printf("radio set pwr -2\r\n");
    read_line(); 
    //set bandwidth to 500KHz
    printf("radio set bw 500\r\n");
    read_line(); 
    //set spread factor to sf7
    printf("radio set sf sf7\r\n");
    read_line(); 
    //set coding rate to 4/8
    printf("radio set cr 4/8\r\n");
    read_line(); 
    //enable crc
    printf("radio set crc on\r\n");
    read_line();
    //read and save uuid
    getUUID();    
}

/**
 * Setup OPT3001 sensor
 */
void InitSensor(void){
    ANSELCbits.ANSC4=0;
    I2C_Init();  
    I2C_Start();
    I2C_Write_Byte(0x88);
    I2C_Write_Byte(0x01);
    I2C_Write_Byte(0xC4);
    I2C_Write_Byte(0x10);
    I2C_Stop();
}

/**
 * Main code 
 * There is a loop after initialization where data from OPT3001 are gathered.
 * Gathered illumination value is then sent via RN2483 module.
 */
void main(void) {
    //System initialization
    SYSTEM_Initialize(); 	
    INTERRUPT_GlobalInterruptEnable();  
	INTERRUPT_PeripheralInterruptEnable();    
    InitRadio();
    InitSensor();
    
    //loop
    while(1){        
        //reads msb and lsb of illumination valu from OPT3001
        I2C_Start();
        I2C_Write_Byte(0x88);
        I2C_Write_Byte(0x00);
        I2C_Start();
        I2C_Write_Byte(0x89);
        uint8_t msb = I2C_Read_Byte(1);
        uint8_t lsb = I2C_Read_Byte(0);
        I2C_Stop();
        //compute crc
        uint8_t crc = msb ^ lsb;
        
        //transmit packet with illumination value
        printf("radio tx 41%s%02X%02X%02X%02X0000\r\n",uuid,inc++,msb,lsb,crc); 
        read_line(); 
        read_line();
        
        //sleeps for 500ms + rand(0..2)*5ms
        wait_5ms(100);        
        wait_5ms((rand()%3));
    }
  
    return;
}

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


void wait_for_ok(void){
    while(uartState!=1){
        __delay_ms(1);
    }
    uartState=0;
}

void wait_5ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(5); }   
}

void wait_1ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(1); }   
}

void getUUID(){
    printf("sys get hweui\r\n");
    read_line(); 
    strcpy(uuid,line); 
}

void InitRadio(void){
    wait_5ms(5); 
    printf("sys reset\r\n");
    wait_5ms(5); 
    printf("sys get ver\r\n");
    read_line(); 
    printf("radio get sf\r\n");
    read_line();  
    printf("radio get bw\r\n");
    read_line();  
    printf("radio get cr\r\n");
    read_line();  
    printf("radio get prlen\r\n");
    read_line(); 
    printf("radio get pwr\r\n");
    read_line(); 
    printf("mac pause\r\n");
    read_line(); 
           
    printf("radio set pwr -2\r\n");
    read_line(); 
    printf("radio set bw 500\r\n");
    read_line(); 
    printf("radio set sf sf7\r\n");
    read_line(); 
    printf("radio set cr 4/8\r\n");
    read_line(); 
    printf("radio set crc on\r\n");
    read_line();
    
    getUUID();
     
    srand((int)strtol(uuid, NULL, 0));
    int delay = 2000+(rand()%1000);
    printf("radio set wdt %d\r\n",delay);
    read_line(); 
}

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

void main(void) {
    SYSTEM_Initialize(); 	
    INTERRUPT_GlobalInterruptEnable();  
	INTERRUPT_PeripheralInterruptEnable();
    
    InitRadio();
    InitSensor();
    
    
    while(1){
        
        I2C_Start();
        I2C_Write_Byte(0x88);
        I2C_Write_Byte(0x00);
        I2C_Start();
        I2C_Write_Byte(0x89);
        uint8_t msb = I2C_Read_Byte(1);
        uint8_t lsb = I2C_Read_Byte(0);
        I2C_Stop();
        uint8_t crc = msb ^ lsb;
        
        
        //delay = 2000+(rand()%1000);
        printf("radio tx 41%s%02X%02X%02X%02X0000\r\n",uuid,inc++,msb,lsb,crc); 
        read_line(); 
        read_line();
        
        
        wait_5ms(200);        
        wait_5ms((rand()%4));
    }
  
    return;
}

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

void process_data_opt(uint8_t msb, uint8_t lsb){
    int16_t raw;
    uint16_t exponent = 0;
	uint32_t result = 0;
    uint32_t Illumination = 0;
    
    
    raw = ((msb << 8) | lsb);

	result = raw&0x0FFF;
	exponent = (raw>>12)&0x000F;

	switch(exponent){
		case 0: //*0.015625
			Illumination = result>>6;
			break;
		case 1: //*0.03125
			Illumination = result>>5;
			break;
		case 2: //*0.0625
			Illumination = result>>4;
			break;
		case 3: //*0.125
			Illumination = result>>3;
			break;
		case 4: //*0.25
			Illumination = result>>2;
			break;
		case 5: //*0.5
			Illumination = result>>1;
			break;
		case 6:
			Illumination = result;
			break;
		case 7: //*2
			Illumination = result<<1;
			break;
		case 8: //*4
			Illumination = result<<2;
			break;
		case 9: //*8
			Illumination = result<<3;
			break;
		case 10: //*16
			Illumination = result<<4;
			break;
		case 11: //*32
			Illumination = result<<5;
			break;
	}
    printf("> %d\r\n",Illumination);

}


void main(void) {
    SYSTEM_Initialize(); 	
    INTERRUPT_GlobalInterruptEnable();  
	INTERRUPT_PeripheralInterruptEnable();
    ANSELCbits.ANSC4=0;

    I2C_Init();
    
    I2C_Start();
    I2C_Write_Byte(0x88);
    I2C_Write_Byte(0x01);
    I2C_Write_Byte(0xC4);
    I2C_Write_Byte(0x10);
    I2C_Stop();
    
    __delay_ms(1); 

    while(1){
        
        I2C_Start();
        I2C_Write_Byte(0x88);
        I2C_Write_Byte(0x7E);
        I2C_Start();
        I2C_Write_Byte(0x89);
        uint8_t msb = I2C_Read_Byte(1);
        uint8_t lsb = I2C_Read_Byte(0);
        I2C_Stop();
        printf("%x %x\r\n",msb,lsb);
        process_data_opt(msb,lsb);
        
        wait_5ms(200);
        
        
    }
    
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
    
   
    /*
    printf("radio set pwr -3\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set mod fsk\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set bt 1.0\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set rxbw 2.6\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    */
            
    printf("sys sleep 100000\r\n");
    read_line();
    
   
    
    return;
}

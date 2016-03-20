#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

void I2C_Init();
void I2C_Start();
void I2C_Stop();
void I2C_Write_Byte(char byt);
char I2C_Read_Byte(char ack);
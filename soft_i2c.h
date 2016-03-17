#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

void Soft_I2C_Init();
void Soft_I2C_Start();
void Soft_I2C_Stop();
void Soft_I2C_WriteByte(char byt);
char Soft_I2C_ReadByte(char ack);

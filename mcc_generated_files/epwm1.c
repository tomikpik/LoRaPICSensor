/**
  ECCP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eccp1.c

  @Summary
    This is the generated driver implementation file for the ECCP1 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for ECCP1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - v3.00
        Device            :  PIC18F26K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

/**
  Section: Included Files
*/

#include <xc.h>
#include "epwm1.h"

/**
  Section: Macro Declarations
*/

#define PWM1_INITIALIZE_DUTY_VALUE    0

/**
  Section: EPWM Module APIs
*/

void EPWM1_Initialize (void)
{
    // Set the PWM to the options selected in MPLAB(c) Code Configurator
    
    // CCP1M P1A,P1C: active low; P1B,P1D: active high; DC1B 0; P1M single; 
    CCP1CON = 0x0E;
    
    // CCP1ASE operating; PSS1BD low; PSS1AC low; CCP1AS disabled; 
    ECCP1AS = 0x00;
    
    // P1RSEN automatic_restart; P1DC 2; 
    PWM1CON = 0x82;
    
    // STR1D P1D_to_port; STR1C P1C_to_port; STR1B P1B_to_port; STR1A P1A_to_CCP1M; STR1SYNC start_at_begin; 
    PSTR1CON = 0x01;
    
    // CCPR1L 0; 
    CCPR1L = 0x00;
    
    // CCPR1H 0; 
    CCPR1H = 0x00;
    
    
    // Selecting Timer2
    CCPTMRS0bits.C1TSEL = 0x0;
}

void EPWM1_LoadDutyValue(uint16_t dutyValue)
{
   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR1L = ((dutyValue & 0x03FC)>>2);
    
   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP1CON = (CCP1CON & 0xCF) | ((dutyValue & 0x0003)<<4);
}
/**
 End of File
*/

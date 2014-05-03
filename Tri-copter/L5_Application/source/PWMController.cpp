/*
 * ServoTask.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: sloretz
 */

#include "PWMController.hpp"
#include "lpc_sys.h"
#include <cstdio>

    //Help from http://openlpc.com/4e26f1/examples/pwm.lpc17xx
    //and lpc_pwm.hpp/cpp

bool PWMController::init()
{
    return true;
}

PWMController::PWMController():
    enableMask(0)
{
    lpc_pconp(pconp_pwm1, true);
    lpc_pclk(pclk_pwm1, clkdiv_1);

    //Reset and clear any interrupts
    LPC_PWM1->TCR = 2;
    LPC_PWM1->IR = 0xff;

    //1 microsecond resolution
    //LPC_PWM1->PR = ((sys_get_cpu_clock()*(1000000)))-1;


    //reset on mr0
    LPC_PWM1->MCR = 1 << 1;

    //MR0 holds the PWM period. For 50Hz this is 20ms
    LPC_PWM1->MR0 = sys_get_cpu_clock()/50; //for 125microseconds

    LPC_PWM1->TCR = (1 << 0) | (1 << 3); // Enable PWM1
    LPC_PWM1->TC = 0; //reset counter
    LPC_PWM1->CTCR &= ~(0xF << 0);
}


//Begin PWM output on this port
bool PWMController::enablePort(pwmType port)
{
    //Enable the match 0 register
    enableMask |= 1;

    if (port >= pwm1 && port <= pwm6)
    {
        //enable the port
        enableMask |= 1 << (1 + port);

        // Pinsel the PWM
        LPC_PINCON->PINSEL4 &= ~(3 << (port*2));
        LPC_PINCON->PINSEL4 |=  (1 << (port*2));

        // Enable the PWM (bits 9-14)
        //Single edge operation
        LPC_PWM1->PCR |= (1 << (port + 9));

        return true;
    }
    return false;
}


void PWMController::setNextPosition(pwmType port, float orientation)
{
    uint32_t pulseWidth;
    //(-1.0f,1000) (1.0f,2000)
    //m = 1000 / 2 = 500
    //b = 1000 + 500 = 1500
   // pulseWidth = 500.0f * orientation + 1500;
    pulseWidth = sys_get_cpu_clock() *.001f *((orientation/2)+1.5f);
    switch(port)
    {
        case pwm1: LPC_PWM1->MR1 = pulseWidth; break;
        case pwm2: LPC_PWM1->MR2 = pulseWidth; break;
        case pwm3: LPC_PWM1->MR3 = pulseWidth; break;
        case pwm4: LPC_PWM1->MR4 = pulseWidth; break;
        case pwm5: LPC_PWM1->MR5 = pulseWidth; break;
        case pwm6: LPC_PWM1->MR6 = pulseWidth; break;
        default:
            break;
    }

    //enable pwm
    LPC_PWM1->LER = enableMask;
}

void PWMController::setPercent(pwmType port, float percent)
{
    uint32_t pulseWidth = sys_get_cpu_clock() * ( (percent/100 * (MAX_PULSE - MIN_PULSE) ) + MIN_PULSE );

    switch(port)
    {
        case pwm1: LPC_PWM1->MR1 = pulseWidth; break;
        case pwm2: LPC_PWM1->MR2 = pulseWidth; break;
        case pwm3: LPC_PWM1->MR3 = pulseWidth; break;
        case pwm4: LPC_PWM1->MR4 = pulseWidth; break;
        case pwm5: LPC_PWM1->MR5 = pulseWidth; break;
        case pwm6: LPC_PWM1->MR6 = pulseWidth; break;
        default:
            break;
    }

    //enable pwm
    LPC_PWM1->LER = enableMask;
}


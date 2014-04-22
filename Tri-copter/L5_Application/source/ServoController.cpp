/*
 * ServoController.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: Michael
 */

#include "ServoController.hpp"

void ServoController::setPercent(pwmType port, float percent)
{
    //TODO: change this to something that makes sense

    uint32_t pulseWidth;
    //(-1.0f,1000) (1.0f,2000)
    //m = 1000 / 2 = 500
    //b = 1000 + 500 = 1500
    // pulseWidth = 500.0f * orientation + 1500;
    //pulseWidth = sys_get_cpu_clock() *.001f *((orientation/2)+1.5f);
   // pulseWidth = sys_get_cpu_clock() * .001f *
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
    LPC_PWM1->LER = 1 << enableMask;
}


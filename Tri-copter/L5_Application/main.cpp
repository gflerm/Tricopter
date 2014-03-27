/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

#include "tasks.hpp"
#include "uart0_min.h"
#include "queue.h"
#include "io.hpp"
#include <stdio.h>
#include <math.h>
#include "ServoController.hpp"
#include "acceleration_sensor.hpp"

ServoController *g_servos = 0;

void accel_task(void *)
{
    //Get orientation from accelerometer
    //Put that orientation onto a queue
    int16_t x,y,z;
    Acceleration_Sensor &as = Acceleration_Sensor::getInstance();
    float orientation;
    float magnitude;
    float axisVal;
    while (1)
    {
        x = as.getX();
        y = as.getY();
        z = as.getZ();

        magnitude = sqrt(x*x + y*y + z*z);
        axisVal = y;

        //z positive is up
        //orientation is a value between -1.0f, and 1.0f
        //1.0f means x faces up
        //-1.0f means x faces down

        orientation = (axisVal / magnitude);
        //printf("Got orientation %f\n", orientation);

        //set the next servo position
        g_servos->setNextPosition(ServoController::pwm1, orientation);

        vTaskDelay(20);
    }
}

void debug_trigger(void *)
{
    const uint16_t TRIG_PIN = (1 << 6);
    LPC_GPIO2->FIODIR |= TRIG_PIN; //p2.6 is output
    //toggle pin voltage every 20 ms
    while (1)
    {
        LPC_GPIO2->FIOSET = TRIG_PIN;
        vTaskDelay(10);
        LPC_GPIO2->FIOCLR = TRIG_PIN;
        vTaskDelay(10);
    }
}

int main(void)
{
    terminalTask *term = new terminalTask(PRIORITY_HIGH);
    g_servos = new ServoController();
    TaskHandle_t accelHandle;
    TaskHandle_t debugHandle;

    scheduler_add_task(term);

    g_servos->enablePort(ServoController::pwm1);

    xTaskCreate(accel_task, "accel_fetch", 1024, 0, PRIORITY_MEDIUM, &accelHandle);
    xTaskCreate(debug_trigger, "trig", 256, 0, PRIORITY_MEDIUM, &debugHandle);


    scheduler_start(true);
    return -1;
}

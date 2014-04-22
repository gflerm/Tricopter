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
//#include "ServoController.hpp"
//#include "acceleration_sensor.hpp"
#include "utilities.h"
#include "task.h"
#include "10dof.h"
#include "10dof.hpp"
#include "BM_Controller.hpp"
#include "OrientationTask.hpp"
#include <cmath>

using namespace _10dof;

//ServoController *g_servos = 0;

/*void accel_task(void *)
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
        delay_ms(1);
        LPC_GPIO2->FIOCLR = TRIG_PIN;
        delay_ms(1);
    }
}
*/
//static TaskHandle_t calibration;

//NEED A BETTER SOLUTION FOR KEEPING THIS!
TaskHandle_t calibration;
TaskHandle_t display;
TaskHandle_t bm_calibraiton;
int main(void)
{
    balance Main_axis;

    OrientationTask* orientation = new OrientationTask();
    MotorControlTask* control = new MotorControlTask(orientation->get_orientation(), orientation->get_height());

	terminalTask *term = new terminalTask(PRIORITY_MEDIUM);
    //g_servos = new ServoController();
    //TaskHandle_t accelHandle;
    //TaskHandle_t debugHandle;

    scheduler_add_task(term);
    scheduler_add_task(orientation);
   // xTaskCreate(user_calibration, "calibrate", 1024, &Main_axis, PRIORITY_CRITICAL, &calibration);

    //g_servos->enablePort(ServoController::pwm1);

    //xTaskCreate(accel_task, "accel_fetch", 1024, 0, PRIORITY_HIGH, &accelHandle);
    //xTaskCreate(debug_trigger, "trig", 256, 0,PRIORITY_HIGH, &debugHandle);

	//vTaskStartScheduler();
    scheduler_start(true);

    delete orientation;
    delete control;
    return -1;
}

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
#include <cmath>

using namespace _10dof;

typedef struct {
        Accelerometer* accel_sensor;
        Gyroscope* gyro_sensor;
        three_axis_info_t* orientation;
} orientation_data_t;

const float ORIENTATION_UPDATE_TIME =  20 / 100000; //ms converted to seconds

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

//<<TODO>>: figure out if the sensors are returning radians or degrees
//parameter: orientation_data_t struct
void update_orientation(void* orientation_data)
{
    const float FILTER_PERCENT_HIGH = .98;
    const float FILTER_PERCENT_LOW = .02;

    //These should be set depending upon the output range of the accelerometer
    //i.e., if the output range is 10 bits and the g-range is +-2g then a reasonable
    //value would be between ~640 (.5g on only one axis) to ~2304 (3g over all axes)
    //
    const uint16_t ACCEL_MAGNITUDE_LOW = 640;
    const uint16_t ACCEL_MAGNITUDE_HIGH = 2304;

    three_axis_info_t accel_data;
    three_axis_info_t gyro_data;
    three_axis_info_t accel_calc;
    three_axis_info_t orientation;

    TickType_t startTime;
    uint16_t accel_magnitude;

    while (1)
    {
        startTime = xTaskGetTickCount();

        //Grab info from accelerometer and gyroscope
        accel_data = (((orientation_data_t*)orientation_data)->accel_sensor)->getXYZ();
        gyro_data = (((orientation_data_t*)orientation_data)->gyro_sensor)->getXYZ();
        orientation = *(((orientation_data_t*)orientation_data)->orientation);

        //Integrate gyroscope data
        orientation.x.word += (gyro_data.x.word) * ORIENTATION_UPDATE_TIME;
        orientation.y.word += (gyro_data.y.word) * ORIENTATION_UPDATE_TIME;
        orientation.z.word += (gyro_data.z.word) * ORIENTATION_UPDATE_TIME;

        //Find the angle from the acceleration data
        accel_calc.x.word = atan2(accel_data.z.word, accel_data.x.word);
        accel_calc.y.word = atan2(accel_data.z.word, accel_data.y.word);

        //If we can trust the accelerometer data, then add it in
        accel_magnitude = abs(accel_data.x.word) + abs(accel_data.y.word) + abs(accel_data.z.word);
        if ( accel_magnitude > ACCEL_MAGNITUDE_LOW && accel_magnitude < ACCEL_MAGNITUDE_HIGH)
        {
            //Complementary filter
            //High pass filter the gyro data, low pass filter the acceleration data
            orientation.x.word = (FILTER_PERCENT_HIGH * (orientation.x.word + gyro_data.x.word * ORIENTATION_UPDATE_TIME)) +
                            (FILTER_PERCENT_LOW  * accel_calc.x.word);
            orientation.y.word = (FILTER_PERCENT_HIGH * (orientation.y.word + gyro_data.y.word * ORIENTATION_UPDATE_TIME)) +
                            (FILTER_PERCENT_LOW * accel_calc.y.word);
            //orientation.z = <<TODO>> use magnetometer if this drifts
        }

        *(((orientation_data_t*)orientation_data)->orientation) = orientation;

        //Delay for a fixed period so the integral code works out
        vTaskDelayUntil(&startTime, (ORIENTATION_UPDATE_TIME * 1000 / portTICK_PERIOD_MS));
    }
}

//NEED A BETTER SOLUTION FOR KEEPING THIS!
TaskHandle_t calibration;
TaskHandle_t display;
TaskHandle_t bm_calibraiton;
TaskHandle_t update_orientation_task;
int main(void)
{
	motors Main_motors;
	balance Main_axis;
	three_axis_info_t orientation;
	Accelerometer &accel_sensor = Accelerometer::getInstance();
	Gyroscope &gyro_sensor = Gyroscope::getInstance();

	accel_sensor.init();
	gyro_sensor.init();

	orientation_data_t orientation_data;
    orientation_data.gyro_sensor = &gyro_sensor;
	orientation_data.accel_sensor = &accel_sensor;
	orientation_data.orientation = &orientation;

	terminalTask *term = new terminalTask(PRIORITY_MEDIUM);
    //g_servos = new ServoController();
    //TaskHandle_t accelHandle;
    //TaskHandle_t debugHandle;

    scheduler_add_task(term);
    xTaskCreate(user_calibration, "calibrate", 1024, &Main_axis, PRIORITY_CRITICAL, &calibration);
    xTaskCreate(calibrate_esc, "bm_calibrate", 512, &Main_motors, PRIORITY_CRITICAL, &bm_calibraiton);

    xTaskCreate(update_orientation, "update_orientation", 4096, &orientation_data, PRIORITY_CRITICAL, &update_orientation_task);
    //g_servos->enablePort(ServoController::pwm1);

    //xTaskCreate(accel_task, "accel_fetch", 1024, 0, PRIORITY_HIGH, &accelHandle);
    //xTaskCreate(debug_trigger, "trig", 256, 0,PRIORITY_HIGH, &debugHandle);

	//vTaskStartScheduler();
    scheduler_start(true);
    return -1;
}

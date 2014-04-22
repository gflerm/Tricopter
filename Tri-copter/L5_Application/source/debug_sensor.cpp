/*
 * debug_sensor.c
 *
 *  Created on: Apr 18, 2014
 *      Author: sloretz
 */

#include "debug_sensor.h"
#include "10dof.hpp"
#include <cstdio>

using namespace _10dof;

void some_task(void*)
{
    //do init stuff
    while (1)
    {
        //do stuff in a loop forever
        vTaskDelay(1000); //Wait 1 second
    }
    //never return
}

void debug_sensor_task(void *)
{
    //Get orientation from accelerometer
    //Put that orientation onto a queue
    axis_info_t x,y,z;
    three_axis_info_t reading;
    Accelerometer &as = Accelerometer::getInstance();
    Gyroscope &gs = Gyroscope::getInstance();
    as.init();
    gs.init();

    vTaskDelay(1000);

    while (1)
    {
        x = as.getX();
        y = as.getY();
        z = as.getZ();

        reading = as.getXYZ();

        printf("Indiv acceleration %d, %d, %d \n", x.word, y.word, z.word);
        printf(" Comb acceleration %d, %d, %d \n", reading.x.word, reading.y.word, reading.z.word);

        x = gs.getX();
        y = gs.getY();
        z = gs.getZ();

        reading = gs.getXYZ();

        printf("Indiv gyro %d, %d, %d \n", x.word, y.word, z.word);
        printf(" Comb gyro %d, %d, %d \n", reading.x.word, reading.y.word, reading.z.word);
        printf("who is gyro %x\n", gs.whoami());


        vTaskDelay(500);

    }
}



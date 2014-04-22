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
#include "debug_sensor.h"

using namespace _10dof;





//NEED A BETTER SOLUTION FOR KEEPING THIS!
TaskHandle_t calibration;
TaskHandle_t display;
TaskHandle_t bm_calibraiton;
TaskHandle_t update_orientation_task;
TaskHandle_t debug_sensor_handle;
int main(void)
{

	terminalTask *term = new terminalTask(PRIORITY_MEDIUM);


    scheduler_add_task(term);

    xTaskCreate(debug_sensor_task, "debug_sen", 1024, 0, PRIORITY_HIGH, &debug_sensor_handle);

    scheduler_start(true);
    return -1;
}

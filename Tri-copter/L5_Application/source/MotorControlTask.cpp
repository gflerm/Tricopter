/*
 * MotorControl.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "MotorControlTask.hpp"

bool MotorControlTask::init()
{
    setRunDuration(MOTOR_CONTROL_UPDATE);
    return true;
}

bool MotorControlTask::run(void* p)
{
    //Roll control
    //  Adjust speed of appropriate front motor


    //Pitch control
    //  Adjust speed of both front motors


    //Yaw control
    //  Adjust servo position


    return true;
}



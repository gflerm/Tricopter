/*
 * MotorControl.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "MotorControlTask.hpp"
#include <algorithm> //max
#include <cmath> //abs

bool MotorControlTask::init()
{
    killed = false;

    ZERO.x = 0;
    ZERO.y = 0;
    ZERO.z = 0;

    setRunDuration(MOTOR_CONTROL_UPDATE);

    pwm_control.enablePort(frontLeftMotor); //p2.0
    pwm_control.enablePort(frontRightMotor); //p2.1
    pwm_control.enablePort(backCenterMotor); //p2.2
    pwm_control.enablePort(backCenterServo); //p2.4

    //Initialize speeds and servo positions
    //TODO: is there an initial startup procedure for the ESCs?

    frontLeftMotorPercent = 15;
    frontRightMotorPercent = 15;
    backCenterMotorPercent = 15;
    backCenterServoPercent = 50;
    heightScalar = 1;

    pwm_control.setPercent(frontLeftMotor, frontLeftMotorPercent);
    pwm_control.setPercent(frontRightMotor, frontRightMotorPercent);
    pwm_control.setPercent(backCenterMotor, backCenterMotorPercent);
    pwm_control.setPercent(backCenterServo, backCenterServoPercent);

    return true;
}

void MotorControlTask::updateMotorServoControl()
{
    //Orientation roll (x): right positive, left negative
    //Orientation pitch (y): up positive, down negative,
    //Orientation yaw (z): ccw positive, cw negative

    //Fill orientation with data from the queue
    //  If there's no new data, we don't care and proceed with data
    //   from the last time, but this shouldn't happen
    xQueueReceive(orientation_queue, &orientation, 0);

    //Roll control
    //  Adjust speed of appropriate front motor
    //  We have a choice of either decreasing one or increasing the other
    //  Try to be smart about it, and increase if we're below height target, decrease otherwise
    if (orientation.x > ZERO.x)
    {
        if (orientation.height > HEIGHT_TARGET)
            frontLeftMotorPercent -= SENSITIVITY_X;
        else
            frontRightMotorPercent += SENSITIVITY_X;
    }
    else if (orientation.x < ZERO.x)
    {
        if (orientation.height > HEIGHT_TARGET)
            frontRightMotorPercent -= SENSITIVITY_X;
        else
            frontLeftMotorPercent += SENSITIVITY_X;
    }

    //Pitch control
    //  Adjust speed of both front motors
    if (orientation.y < ZERO.y)
    {
        frontLeftMotorPercent += SENSITIVITY_Y;
        frontRightMotorPercent += SENSITIVITY_Y;
    }
    else if (orientation.y > ZERO.y)
    {
        frontLeftMotorPercent -= SENSITIVITY_Y;
        frontRightMotorPercent -= SENSITIVITY_X;
    }

    //Yaw control
    //  Adjust servo position, maybe adjust back motor speed?
    if (orientation.z < ZERO.z)
    {
        if (backCenterServoPercent - SENSITIVITY_Z > PERCENT_MIN)
            backCenterServoPercent -= SENSITIVITY_Z;
    }
    else if (orientation.z > ZERO.z)
    {
        if (backCenterServoPercent + SENSITIVITY_Z < PERCENT_MAX)
            backCenterServoPercent += SENSITIVITY_Z;
    }

   //Height control
   //  scale all percents by whether we are too low/high
   if (orientation.height < HEIGHT_TARGET)
   {
       heightScalar += SENSITIVITY_HEIGHT;
   }
   else if (orientation.height > HEIGHT_TARGET)
   {
       heightScalar -= SENSITIVITY_HEIGHT;
   }

   //For motor controls, make sure we don't exceed 100% after height scaling
   pwm_control.setPercent(frontRightMotor, std::min(PERCENT_MAX, frontRightMotorPercent * heightScalar));
   pwm_control.setPercent(frontLeftMotor, std::min(PERCENT_MAX, frontLeftMotorPercent * heightScalar));
   pwm_control.setPercent(backCenterMotor, std::min(PERCENT_MAX, backCenterMotorPercent * heightScalar));
   pwm_control.setPercent(backCenterServo, backCenterServoPercent);
}

bool MotorControlTask::violatesFailSafe()
{
    if (std::abs(orientation.x) > MAX_X_ANGLE)
        return true;
    if (std::abs(orientation.y) > MAX_Y_ANGLE)
        return true;
    if (orientation.height > MAX_HEIGHT)
        return true;

    return false;
}

void MotorControlTask::killMotors()
{
    pwm_control.setPercent(frontRightMotor, 0);
    pwm_control.setPercent(frontLeftMotor, 0);
    pwm_control.setPercent(backCenterMotor, 0);
}

bool MotorControlTask::run(void* p)
{
    if (!killed)
    {
        if (! (killed = violatesFailSafe()))
            updateMotorServoControl();
        else
            killMotors();
    }

    return true;
}



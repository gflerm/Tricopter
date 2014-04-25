/*
 * MotorControl.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "MotorControlTask.hpp"
#include <algorithm> //max

bool MotorControlTask::init()
{
    ZERO.x = 0;
    ZERO.y = 0;
    ZERO.z = 0;

    setRunDuration(MOTOR_CONTROL_UPDATE);

    motor_control.enablePort(frontLeftMotor); //p2.0
    motor_control.enablePort(frontRightMotor);
    motor_control.enablePort(backCenterMotor); //p2.3
    servo_control.enablePort(backCenterServo);

    //Initialize speeds and servo positions
    //TODO: is there an initial startup procedure for the ESCs?

    frontLeftMotorPercent = 15;
    frontRightMotorPercent = 15;
    backCenterMotorPercent = 15;
    backCenterServoPercent = 50;
    heightScalar = 1;

   /* motor_control.setPercent(frontLeftMotor, frontLeftMotorPercent);
    motor_control.setPercent(frontRightMotor, frontRightMotorPercent);
    motor_control.setPercent(backCenterMotor, backCenterMotorPercent);
    servo_control.setPercent(backCenterServo, backCenterServoPercent); */

    motor_control.setNextPosition(frontLeftMotor, -1.0f);
    motor_control.setNextPosition(frontRightMotor, .5f);
    motor_control.setNextPosition(backCenterMotor, 1.0f);
    motor_control.setNextPosition(backCenterServo, .5f);
    return true;
}

bool MotorControlTask::run(void* p)
{
    //Orientation roll (x): right positive, left negative
    //Orientation pitch (y): up positive, down negative,
    //Orientation yaw (z): ccw positive, cw negative

    //Roll control
    //  Adjust speed of appropriate front motor
    //  if orientation to the right, increase FR motor

    /*
    if (orientation->x > ZERO.x)
    {
        frontRightMotorPercent += SENSITIVITY_X;
        frontLeftMotorPercent -= SENSITIVITY_X;
    }
    //  if orientation to the left, increase FL motor
    else if (orientation->x < ZERO.x)
    {
        frontLeftMotorPercent += SENSITIVITY_X;
        frontRightMotorPercent -= SENSITIVITY_X;
    }

    //Pitch control
    //  Adjust speed of both front motors
    if (orientation->y < ZERO.y)
    {
        frontLeftMotorPercent += SENSITIVITY_Y;
        frontRightMotorPercent += SENSITIVITY_Y;
    }
    else if (orientation-> y > ZERO.y)
    {
        frontLeftMotorPercent -= SENSITIVITY_Y;
        frontRightMotorPercent -= SENSITIVITY_X;
    }

    //Yaw control
    //  Adjust servo position, maybe adjust back motor speed?
    if (orientation->z < ZERO.z)
    {
        if (backCenterServoPercent - SENSITIVITY_Z > PERCENT_MIN)
            backCenterServoPercent -= SENSITIVITY_Z;
    }
    else if (orientation-> z > ZERO.z)
    {
        if (backCenterServoPercent + SENSITIVITY_Z < PERCENT_MAX)
            backCenterServoPercent += SENSITIVITY_Z;
    }

    //Height control
    //  scale all percents by whether we are too low/high
    if ((*height) < HEIGHT_TARGET)
    {
        heightScalar += SENSITIVITY_HEIGHT;
    }
    else if ((*height) > HEIGHT_TARGET)
    {
        heightScalar -= SENSITIVITY_HEIGHT;
    }

    motor_control.setPercent(frontRightMotor, std::max(PERCENT_MAX, frontRightMotorPercent * heightScalar));
    motor_control.setPercent(frontLeftMotor, std::max(PERCENT_MAX, frontLeftMotorPercent * heightScalar));
    motor_control.setPercent(backCenterMotor, std::max(PERCENT_MAX, backCenterMotorPercent * heightScalar));
    servo_control.setPercent(backCenterServo, backCenterServoPercent);

    */

    return true;
}



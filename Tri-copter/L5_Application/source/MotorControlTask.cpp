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

    motor_control.enablePort(frontLeftMotor);
    motor_control.enablePort(frontRightMotor);
    motor_control.enablePort(backCenterMotor);

    servo_control.enablePort(backCenterServo);

    //Initialize speeds and servo positions
    //TODO: is there an initial startup procedure for the ESCs?

    motor_control.setPercent(frontLeftMotor, 0);
    motor_control.setPercent(frontRightMotor, 0);
    motor_control.setPercent(backCenterMotor, 0);
    servo_control.setPercent(backCenterServo, 50);
    return true;
}

bool MotorControlTask::run(void* p)
{
    //Roll control
    //  Adjust speed of appropriate front motor

    //if orientation to the right, increase FR motor
    motor_control.setPercent(frontRightMotor, 0);

    //if orientation to the left, increase FL motor
    motor_control.setPercent(frontLeftMotor, 0);

    //Pitch control
    //  Adjust speed of both front motors
    motor_control.setPercent(frontLeftMotor, 0);
    motor_control.setPercent(frontRightMotor, 0);

    //Yaw control
    //  Adjust servo position, maybe adjust back motor speed?
    servo_control.setPercent(backCenterServo, 0);

    return true;
}



/*
 * MotorControl.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#ifndef MOTORCONTROL_HPP_
#define MOTORCONTROL_HPP_

#include "scheduler_task.hpp"
#include "ServoController.hpp"
#include "10dof.hpp"

using namespace _10dof;

//Controls the motor speed and servo position based on the orientation data
//Makes some assumptions:
//  a. X axis = roll  (TODO: this might be switched around???)
//  b. Y axis = pitch ( ^^^ ??)
//  c. Z axis = yaw
//  d. Left front motor PWM port   1
//  e. Right front motor PWM port  2
//  f. Back motor PWM port         3
//  g. Servo PWM port              4
class MotorControlTask : public scheduler_task
{
public:
    //MotorControlTask()
    //Supplies some default values to the scheduler_task constructor
    MotorControlTask(orientation_t* _orientation, float* _height)
                   : scheduler_task("motor_control_task", STACK_SIZE_BYTES, PRIORITY_HIGH, NULL)
    {
        orientation = _orientation;
        height = _height;
    };

    //init()
    //Initialize the motor control
    bool init();

    //run()
    //Update the motor speed and servo position based on the orientation
    bool run(void* p);
private:
    ServoController motor_control;
    ServoController servo_control;

    //Pointer to orientation and height
    orientation_t* orientation;
    float* height;

    //Task control settings
    static const int STACK_SIZE_BYTES = 4096;
    static const int MOTOR_CONTROL_UPDATE = 20; //ms, we can update the motor control at most 50 times/sec

    //Calibration settings
    static const float SENSITIVITY_X = .05; //scalar for how fast the motors should spin up
    static const float SENSITIVITY_Y = .05;
    static const float SENSITIVITY_Z = .05;
    static const float SENSITIVITY_HEIGHT = .05;

    static const float PERCENT_MAX = 100;
    static const float PERCENT_MIN = 0;

    float frontLeftMotorPercent;
    float frontRightMotorPercent;
    float backCenterMotorPercent;
    float backCenterServoPercent;

    float heightScalar;

    static const ServoController::pwmType frontLeftMotor = ServoController::pwm1;
    static const ServoController::pwmType frontRightMotor = ServoController::pwm2;
    static const ServoController::pwmType backCenterMotor = ServoController::pwm3;
    static const ServoController::pwmType backCenterServo = ServoController::pwm4;

    orientation_t ZERO;
    static const float HEIGHT_TARGET = 12; //inches
};

#endif /* MOTORCONTROL_HPP_ */

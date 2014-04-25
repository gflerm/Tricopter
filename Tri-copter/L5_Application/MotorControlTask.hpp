/*
 * MotorControl.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#ifndef MOTORCONTROL_HPP_
#define MOTORCONTROL_HPP_

#include "scheduler_task.hpp"
#include "PWMController.hpp"
#include "10dof.hpp"

using namespace _10dof;

//Controls the motor speed and servo position based on the orientation data
//Makes some assumptions:
//  a. X axis = roll
//  b. Y axis = pitch
//  c. Z axis = yaw
//  d. Left front motor PWM port   1
//  e. Right front motor PWM port  2
//  f. Back motor PWM port         3
//  g. Servo PWM port              4
//  h. Orientation roll (x): right positive, left negative
//  i. Orientation pitch (y): up positive, down negative,
//  j. Orientation yaw (z): ccw positive, cw negative
class MotorControlTask : public scheduler_task
{
public:
    //MotorControlTask()
    //Supplies some default values to the scheduler_task constructor
    MotorControlTask(QueueHandle_t or_queue)
                   : scheduler_task("motor_control_task", STACK_SIZE_BYTES, PRIORITY_HIGH, NULL)
    {
        orientation_queue = or_queue;
    };

    //init()
    //Initialize the motor control
    bool init();

    //run()
    //Checks fail safe conditions, then updates motor/servo control
    bool run(void* p);
private:
    //checkFailSafe
    //Returns TRUE if fail safe conditions are violated, FALSE otherwise
    bool violatesFailSafe();

    //updateMotorSpeed
    //Updates the speed of the motors and servo position based on orientation
    void updateMotorServoControl();

    //killMotors
    //Sets all motor percents to 0
    void killMotors();

    //Queue where the orientation data is placed
    QueueHandle_t orientation_queue;

    //Controls three motors and servo
    PWMController pwm_control;

    //True if the fail safe conditions are violated and motors have stopped spinning
    bool killed;

    //Last received orientation and height
    orientation_t orientation;

    //Task control settings
    static const int STACK_SIZE_BYTES = 4096;
    static const int MOTOR_CONTROL_UPDATE = 20; //ms, we can update the motor control at most 50 times/sec

    //Maximum orientation angles before we kill the motors
    static const float MAX_X_ANGLE = .78; //45 degrees on x and y, don't really care about z
    static const float MAX_Y_ANGLE = .78;
    static const float MAX_HEIGHT = 18; //inches

    //Calibration settings
    static const float SENSITIVITY_X = .05; //scalar for how fast the motors should spin up
    static const float SENSITIVITY_Y = .05;
    static const float SENSITIVITY_Z = .05;
    static const float SENSITIVITY_HEIGHT = .05;

    //Targets for hovering
    orientation_t ZERO;
    static const float HEIGHT_TARGET = 12; //inches

    static const float PERCENT_MAX = 100;
    static const float PERCENT_MIN = 0;

    float frontLeftMotorPercent;
    float frontRightMotorPercent;
    float backCenterMotorPercent;
    float backCenterServoPercent;

    //Increases/decreases all motor speeds based on height
    float heightScalar;

    static const PWMController::pwmType frontLeftMotor = PWMController::pwm1;
    static const PWMController::pwmType frontRightMotor = PWMController::pwm2;
    static const PWMController::pwmType backCenterMotor = PWMController::pwm3;
    static const PWMController::pwmType backCenterServo = PWMController::pwm4;
};

#endif /* MOTORCONTROL_HPP_ */

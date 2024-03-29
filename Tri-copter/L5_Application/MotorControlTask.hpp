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
//  h. Orientation roll (x): right negative, left positive
//  i. Orientation pitch (y): up negative, down positive,
//  j. Orientation yaw (z): ccw positive, cw negative
class MotorControlTask : public scheduler_task
{
public:
    //MotorControlTask()
    //Supplies some default values to the scheduler_task constructor
    MotorControlTask(QueueHandle_t or_queue)
                   : scheduler_task("motor_control_task", STACK_SIZE_BYTES, PRIORITY_MEDIUM, NULL)
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
    //Each motor has a base percentage (ex FRONT_RIGHT_PERCENT) upon which the actual speed is calculated
    //  -> First, a correction based on orientation is calculated. This is added to the base percent.
    //  -> The result of that is then scaled by the height scalar depending upon whether we want to be
    //     higher or lower
    void updateMotorServoControl();

    //killMotors
    //Sets all motor percents to 0
    void killMotors();

    //isTimerExpired
    //Returns true if tickCount >= MAX_SEC
    bool isTimerExpired();

    //Returns true if tickCount <= MIN_SEC
    bool isNotReady();

    //Keeps a percentage in range of [PERCENT_MIN_xxx, PERCENT_MAX_xxx]
    float inRangeMotor(float motorPercent);
    float inRangeServo(float servoPercent);

    //getOrientationFromQueue
    //fills orientation variable
    void getOrientationFromQueue();

    //Queue where the orientation data is placed
    QueueHandle_t orientation_queue;

    //True if the fail safe conditions are violated and motors have stopped spinning
    bool killed;
    //Timer for on/off control
    bool paused;

    //Last received orientation and height
    orientation_t orientation;

    //Incremented every time the run() function is called
    int tickCount;

    //Increases/decreases all motor speeds based on height
    float currentHeightTarget;
    float heightPrevious;

    //Accumulative values
    //These are the integral "I" values of the PI controller in updateMotorServoControl()
    float heightScalarAccum;
    float frontRightAccum;
    float frontLeftAccum;
    float backCenterAccum;
    float servoAccum;

    //Task control settings
    static const int STACK_SIZE_BYTES = 4096;

    //How often the motor control task should run
    static const int MOTOR_CONTROL_UPDATE = 20; //ms, we can update the motor control at most 50 times/sec

    //For timer purposes, so we can kill the power after a certain time
    const static int MAX_SEC = 75;
    const static int MIN_SEC = 5;

    //Maximum orientation values before we kill the motors
    static const float MAX_ANGLE = .200; //radians
    static const float MAX_HEIGHT = 24; //inches

    //PWM port assignments
    static const PWMController::pwmType frontLeftMotor = PWMController::pwm1;  //2.0
    static const PWMController::pwmType frontRightMotor = PWMController::pwm2; //2.1
    static const PWMController::pwmType backCenterMotor = PWMController::pwm3; //2.2
    static const PWMController::pwmType backCenterServo = PWMController::pwm4; //2.3

    //~~~~~~~~~~~~~~~~~ CALIBRATION SETTINGS ~~~~~~~~~~~~~~~~~~~~~~~~~
    //PI gains
    static const float PRESENT_GAIN = .80;
    static const float ACCUM_GAIN = .20;

    //Threshold to start accumulation
    static const float ACCUM_MOTOR_THRESHOLD = .075;
    static const float ACCUM_SERVO_THRESHOLD = .075;
    static const float ACCUM_HEIGHT_THRESHOLD = 2;

    //Accumulation strength control
    static const float ACCUM_SCALAR_MOTOR = 25;
    static const float ACCUM_SCALAR_SERVO = 25;
    static const float ACCUM_SCALAR_HEIGHT = 1;

    //These are base percentages that should be set to values which cause the tricopter to
    //almost hover in a somewhat stable position
    static const float FRONT_LEFT_PERCENT = 35;
    static const float FRONT_RIGHT_PERCENT = 49;
    static const float BACK_CENTER_PERCENT = 55.5;
    static const float SERVO_PERCENT = 41;
    static const float HEIGHT_SCALAR = 1;

    //Sensitivity settings
    static const float SENSITIVITY_X = 50; //scalar for how fast the motors should spin up
    static const float SENSITIVITY_Y = 50;
    static const float SENSITIVITY_Z = 250; //servo
    static const float SENSITIVITY_HEIGHT = .05; //percent * 100
    static const float CORRECTION_MOTOR_DEGREE = 1; //1 = linear, 2 = quadratic, etc
    static const float CORRECTION_SERVO_DEGREE = 1;
    static const float CORRECTION_HEIGHT_DEGREE = 1;

    //Targets for hovering
    static const float ZERO_X = .044; //radians
    static const float ZERO_Y = -.020;
    static const float ZERO_Z = 0;
    static const float HOVER_HEIGHT_TARGET = 6; //inches
    static const float HEIGHT_VELOCITY_TARGET = .5; //inches per second

    //Limits for motor speed and servo position
    static const float PERCENT_MAX_MOTOR = 70;
    static const float PERCENT_MIN_MOTOR = 9;
    static const float PERCENT_MAX_SERVO = 100;
    static const float PERCENT_MIN_SERVO = 0;
};

#endif /* MOTORCONTROL_HPP_ */

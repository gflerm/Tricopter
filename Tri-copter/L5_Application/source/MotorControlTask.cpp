/*
 * MotorControl.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "MotorControlTask.hpp"
#include <algorithm> //max
#include <cmath> //abs
#include <cstdio> //debug
#include "pid_controllers.hpp"

//Shane is retarded
#define _toRadians(deg) (deg*M_PI/180.0f)
#define inline_function_snob "Michael"

float MotorControlTask::timeToResolve(float orientation, float target)
{
    //given our orientation and target orientation in radians,
    //How long should we take to resolve the problem?
    //Worst case (10 degrees) 0.5 seconds, best (0.00000... degrees) 0.05 seconds

    float time = .4f / pow(fabs(orientation-target) + .33,CORRECTION_DEGREE);
    if (time < 0.5)
        time = 0.5;
    return time;
}

float MotorControlTask::getSign(float value)
{
    return value > 0 ? 1.0f : -1.0f;
}

float actual_roll_max;
float actual_roll_min;

float target_roll_max;
float target_roll_min;

bool MotorControlTask::init()
{
    PWMController &pwm_control = PWMController::getInstance();
    setRunDuration(MOTOR_CONTROL_UPDATE);

    killed = false;
    paused = false;
    tickCount = 0;

    pwm_control.enablePort(frontLeftMotor);  //p2.0
    pwm_control.enablePort(frontRightMotor); //p2.1
    pwm_control.enablePort(backCenterMotor); //p2.2
    pwm_control.enablePort(backCenterServo); //p2.4

    heightScalar = 1;
    baseMotorPower = 0;
    currentHeightTarget = HOVER_HEIGHT_TARGET;

    //These values will be kept until the time reaches MIN_SEC
    pwm_control.setPercent(frontLeftMotor, 0);
    pwm_control.setPercent(frontRightMotor, 0);
    pwm_control.setPercent(backCenterMotor, 0);
    pwm_control.setPercent(backCenterServo, 45);

    actual_roll_min = 100000;
    actual_roll_max = -100000;

    target_roll_min = 100000;
    target_roll_max = -100000;

    return true;
}



bool MotorControlTask::run(void* p)
{
    tickCount++;
    getOrientationFromQueue();

    if ( !(paused = isNotReady() ) && !killed) //CANNOT enter this block if we've
    {                                          //killed it due to fail safe violation
        if ((!(killed = violatesFailSafe())))  //otherwise it'll restart and maim someone ;_;
        {
            if (isTimerExpired())
            {
                currentHeightTarget = 0; //Try to gracefully land
            }
            updateMotorServoControl();
        }
        else
        {
            killMotors();
        }
    }

    if (killed)
    {
        //printf("Killed, failsafe violation\n");
    }

    return true;
}

float target_roll = 0;
float target_pitch = 0;
float target_yaw = 0;

//These apply a correction to base motor/servo percentage based on orientation
float frontLeftCorrection = 0;
float frontRightCorrection = 0;
float backCenterCorrection = 0;
float servoCorrection = 0;

float actual_roll;
float actual_pitch;
float actual_yaw;

void MotorControlTask::updateMotorServoControl()
{
    PWMController &pwm_control = PWMController::getInstance();
    //millidegrees per second to radians per second
    actual_roll = _toRadians(orientation.gx) / 1000.0f;
    actual_pitch = _toRadians(orientation.gy) / 1000.0f;
    actual_yaw = _toRadians(orientation.gz)/ 1000.0f;

    if (actual_roll > actual_roll_max)
        actual_roll_max = actual_roll;
    if (actual_roll < actual_roll_min)
        actual_roll_min = actual_roll;

    if (target_roll > target_roll_max)
        target_roll_max = target_roll;
    if (target_roll < target_roll_min)
        target_roll_min = target_roll;

    target_roll = -1.0f * (orientation.x - ZERO_X) / (timeToResolve(orientation.x,ZERO_X) * 20);
    target_pitch = (orientation.y - ZERO_Y) / (timeToResolve(orientation.y,ZERO_Y) * 4);
    target_yaw = -1.0f * (orientation.z - ZERO_Z) / (timeToResolve(orientation.z,ZERO_Z) * 10);

    float dt = MOTOR_CONTROL_UPDATE / 1000.0f;
    float roll_output = pid_roll.calculate_output(actual_roll, target_roll, dt);
    float pitch_output = pid_pitch.calculate_output(actual_pitch, target_pitch, dt);
    float yaw_output = pid_yaw.calculate_output(actual_yaw, target_yaw, dt);
   // baseMotorPower = pid_height.calculate_output(orientation.height, currentHeightTarget, dt);
    if (baseMotorPower < 20)
    {
        if (tickCount % 5 == 0)
        {
            baseMotorPower++;
            printf("%i\n", tickCount);
        }
    }
    //baseMotorPower = 20;

    //Determine amount to decrease motors
    //Help from line 126 in
    //https://github.com/diydrones/ardupilot/blob/416e9457ce11ae37200e6380834b1c5f3a4cd2e5/libraries/AP_Motors/AP_MotorsTri.cpp
    frontLeftCorrection = -roll_output + pitch_output/2;
    frontRightCorrection = roll_output + pitch_output/2;
    backCenterCorrection = -pitch_output;
    servoCorrection = yaw_output;

   //Now set the motor percents with the corrections and height scaling applied
   pwm_control.setPercent(frontRightMotor,
                          inRangeMotor( baseMotorPower + frontRightCorrection)
                          );
   pwm_control.setPercent(frontLeftMotor,
                          inRangeMotor( baseMotorPower + frontLeftCorrection)
                          );
   pwm_control.setPercent(backCenterMotor,
                          inRangeMotor( baseMotorPower + backCenterCorrection)
                          );

   //Servo doesn't get height scaling
   pwm_control.setPercent(backCenterServo, inRangeServo(SERVO_PERCENT + servoCorrection));

   //Debug output, delete me
  /* printf("x: %f, y %f, z %f \n", orientation.x, orientation.y, orientation.z);
   printf("ar: %f, ap %f, ay %f \n", actual_roll, actual_pitch, actual_yaw);
   printf("tr: %f, tp %f, ty %f \n", target_roll, target_pitch, target_yaw);
   printf("Height scalar %n", heightScalar);
   printf("Front right motor: %f\n", frontRightCorrection);
   printf("Front left motor: %f\n", frontLeftCorrection);
   printf("Back center motor: %f\n", backCenterCorrection);
   printf("Back center servo: %f \n", servoCorrection);
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); */
}

bool MotorControlTask::violatesFailSafe()
{
    /*
    if (std::abs(orientation.x) > MAX_X_ANGLE) {
        printf("Killed due to orientation.x of %f \n", orientation.x);
        printf("Accel x: %f \n", orientation.ax);
        return true;
    }
    if (std::abs(orientation.y) > MAX_Y_ANGLE) {
        printf("Killed due to orientation.y of %f \n", orientation.y);
        return true;
    }
    if (orientation.height > MAX_HEIGHT) {
        printf("Killed due to height of %f", orientation.height);
        return true;
    }
    */
    return false;
}

void MotorControlTask::killMotors()
{
    PWMController &pwm_control = PWMController::getInstance();
    pwm_control.setPercent(frontRightMotor, 0);
    pwm_control.setPercent(frontLeftMotor, 0);
    pwm_control.setPercent(backCenterMotor, 0);
}

bool MotorControlTask::isTimerExpired()
{
    if (tickCount >= MAX_SEC * 1000 / MOTOR_CONTROL_UPDATE)
        return true;
    else
        return false;
}

bool MotorControlTask::isNotReady()
{
    if (tickCount <= MIN_SEC * 1000 / MOTOR_CONTROL_UPDATE )
        return true;
    else
        return false;
}

float MotorControlTask::inRangeMotor(float motorPercent)
{
    if (motorPercent < PERCENT_MIN_MOTOR)
        return PERCENT_MIN_MOTOR;
    else if (motorPercent > PERCENT_MAX_MOTOR)
        return PERCENT_MAX_MOTOR;
    else
        return motorPercent;
}

float MotorControlTask::inRangeServo(float servoPercent)
{
    if (servoPercent < PERCENT_MIN_SERVO)
        return PERCENT_MIN_SERVO;
    else if (servoPercent > PERCENT_MAX_SERVO)
        return PERCENT_MAX_SERVO;
    else
        return servoPercent;
}

void MotorControlTask::getOrientationFromQueue()
{
    //Fill orientation with data from the queue
    //  If there's no new data, we don't care and proceed with data
    //   from the last time, but this shouldn't happen
    xQueueReceive(orientation_queue, &orientation, 0);
}


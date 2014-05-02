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

#define _toRadians(deg) (deg*M_PI/180.0f)

float timeToResolve(float orientation, float target)
{
    //given our orientation and target orientation in radians,
    //How long should we take to resolve the problem?
    //Worst case (10 degrees) 0.5 seconds, best (0.00000... degrees) 0.05 seconds

    return 0.15f; //TODO
}

float getSign(float value)
{
    return value > 0 ? 1.0f : -1.0f;
}

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
    pwm_control.setPercent(backCenterServo, 50);

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
        printf("Killed, failsafe violation\n");
    }

    return true;
}

void MotorControlTask::updateMotorServoControl()
{
    PWMController &pwm_control = PWMController::getInstance();

    //These apply a correction to base motor/servo percentage based on orientation
    float frontLeftCorrection = 0;
    float frontRightCorrection = 0;
    float backCenterCorrection = 0;
    float servoCorrection = 0;

    //millidegrees per second to radians per second
    float actual_roll = _toRadians(orientation.gx) / 1000.0f;
    float actual_pitch = _toRadians(orientation.gy) / 1000.0f;
    float actual_yaw = _toRadians(orientation.gz)/ 1000.0f;

    float target_roll = 0;
    float target_pitch = 0;
    float target_yaw = 0;

    //  axis x: right positive, left negative
    //  axis y: up positive, down negative,
    //  axis z: counterclockwise positive, clockwise negative
    target_roll = getSign(ZERO_X - orientation.x) * fabs(orientation.x) / timeToResolve(orientation.x,ZERO_X);
    target_pitch = getSign(ZERO_Y - orientation.y) * fabs(orientation.y) / timeToResolve(orientation.y,ZERO_Y);
    target_yaw = getSign(ZERO_Z - orientation.z) * fabs(orientation.z) / timeToResolve(orientation.z,ZERO_Z);

   //Height control
   //  scale all percents by whether we are too low/high
   baseMotorPower += SENSITIVITY_HEIGHT * getSign(currentHeightTarget - orientation.height);

   //Determine amount to decrease motors
   frontLeftCorrection = SENSITIVITY_X * (target_roll - actual_roll);
   frontRightCorrection = SENSITIVITY_X * (actual_roll - target_roll);
   backCenterCorrection = SENSITIVITY_Y * (target_pitch - actual_pitch);
   servoCorrection = SENSITIVITY_Z * (target_yaw - actual_yaw);


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
   printf("x: %f, y %f, z %f \n", orientation.x, orientation.y, orientation.z);
   printf("ar: %f, ap %f, ay %f \n", actual_roll, actual_pitch, actual_yaw);
   printf("tr: %f, tp %f, ty %f \n", target_roll, target_pitch, target_yaw);
   printf("Height scalar %n", heightScalar);
   printf("Front right motor: %f\n", frontRightCorrection);
   printf("Front left motor: %f\n", frontLeftCorrection);
   printf("Back center motor: %f\n", backCenterCorrection);
   printf("Back center servo: %f \n", servoCorrection);
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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


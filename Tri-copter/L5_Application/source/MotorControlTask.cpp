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

bool MotorControlTask::init()
{
    PWMController &pwm_control = PWMController::getInstance();
    setRunDuration(MOTOR_CONTROL_UPDATE);

    killed = false;
    paused = false;
    tickCount = 0;

    frontRightAccum = 0;
    frontLeftAccum = 0;
    backCenterAccum = 0;
    servoAccum = 0;
    heightScalarAccum = 0;

    pwm_control.enablePort(frontLeftMotor);  //p2.0
    pwm_control.enablePort(frontRightMotor); //p2.1
    pwm_control.enablePort(backCenterMotor); //p2.2
    pwm_control.enablePort(backCenterServo); //p2.3

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
    return true;
}

void MotorControlTask::updateMotorServoControl()
{
    //Implements a PI controller
    // > en.wikipedia.org/PID_Controller
    PWMController &pwm_control = PWMController::getInstance();

    //These apply a correction to base motor/servo percentage based on orientation
    float frontLeftCorrection = 0;
    float frontRightCorrection = 0;
    float backCenterCorrection = 0;
    float servoCorrection = 0;
    float heightScalarCorrection = 0;

    //These are the proportional "P" part of the PI controller
    float frontLeftPresent = 0;
    float frontRightPresent = 0;
    float backCenterPresent = 0;
    float servoPresent = 0;
    float heightScalarPresent = 0;

    //Roll control
    //  Adjust the speed of either front motor
    //  axis x: right negative, left positive
    float xErr = orientation.x - ZERO_X;
    float xErrAbs = fabs(xErr);
    if (xErr < 0) //roll right => increase right motor
    {
        if (xErrAbs > ACCUM_MOTOR_THRESHOLD)
        {
            frontRightAccum += ACCUM_SCALAR_MOTOR * xErrAbs;
            frontLeftAccum -= ACCUM_SCALAR_MOTOR * xErrAbs;
        }
        frontRightPresent = SENSITIVITY_X * pow(xErrAbs , CORRECTION_MOTOR_DEGREE);
    }
    else //roll left => increase left motor
    {
        if (fabs(xErr) > ACCUM_MOTOR_THRESHOLD)
        {
            frontLeftAccum += ACCUM_SCALAR_MOTOR * xErrAbs;
            frontRightAccum -= ACCUM_SCALAR_MOTOR * xErrAbs;
        }
        frontLeftPresent = SENSITIVITY_X * pow(xErrAbs, CORRECTION_MOTOR_DEGREE);
    }

    //Pitch control
    //  Increase/decrease speed of back motor
    //  axis y: up negative, down positive
    float yErr = orientation.y - ZERO_Y;
    float yErrAbs = fabs(yErr);
    if (yErr < 0) //pitch up => decrease back motor
    {
        if (fabs(yErr) > ACCUM_MOTOR_THRESHOLD)
        {
            backCenterAccum -= ACCUM_SCALAR_MOTOR * yErrAbs;
        }
        //note the negative
        backCenterPresent = -(SENSITIVITY_Y * pow(yErrAbs, CORRECTION_MOTOR_DEGREE));
    }
    else //pitch down => increase back motor
    {
        if (fabs(yErr) > ACCUM_MOTOR_THRESHOLD)
        {
            backCenterAccum += ACCUM_SCALAR_MOTOR * yErrAbs;
        }
        backCenterPresent = SENSITIVITY_Y * pow(yErrAbs, CORRECTION_MOTOR_DEGREE);
    }

    //Yaw control
    //  Increase or decrease servo
    //  axis z: counterclockwise positive, clockwise negative
    float zErr = orientation.z - ZERO_Z;
    float zErrAbs = fabs(zErr);
    if (zErr > 0) //yaw counter clockwisew => decrase servo
    {
        if (zErrAbs > ACCUM_SERVO_THRESHOLD)
        {
            servoAccum -= ACCUM_SCALAR_SERVO * zErrAbs;
        }
        //note the negative
        servoPresent = -(SENSITIVITY_Z * pow(zErrAbs, CORRECTION_SERVO_DEGREE));
    }
    else //yaw clockwise => increase servo
    {
        if (zErrAbs > ACCUM_SERVO_THRESHOLD)
        {
            servoAccum += ACCUM_SCALAR_SERVO * zErrAbs;
        }
        servoPresent = SENSITIVITY_Z * pow(zErrAbs, CORRECTION_SERVO_DEGREE);
    }

    //Height control
    //  scale all percents by whether we are too low/high and too slow/fast
    float heightErr = orientation.height - currentHeightTarget;
    float heightErrAbs = fabs(heightErr);
    if (heightErr < 0) //too low
    {
        if (heightErrAbs > ACCUM_HEIGHT_THRESHOLD)
        {
            heightScalarAccum += SENSITIVITY_HEIGHT;
        }
        heightScalarPresent = -(SENSITIVITY_HEIGHT * pow(heightErrAbs, CORRECTION_HEIGHT_DEGREE));
    }
    else  //too high
    {
        if (heightErrAbs > ACCUM_HEIGHT_THRESHOLD)
        {
            heightScalarAccum -= SENSITIVITY_HEIGHT;
        }
        heightScalarPresent = (SENSITIVITY_HEIGHT * pow(heightErrAbs, CORRECTION_HEIGHT_DEGREE));
    }

   frontRightCorrection = (PRESENT_GAIN * frontRightPresent) + (ACCUM_GAIN * frontRightAccum);
   frontLeftCorrection = (PRESENT_GAIN * frontLeftPresent) + (ACCUM_GAIN * frontLeftAccum);
   backCenterCorrection = (PRESENT_GAIN * backCenterPresent) + (ACCUM_GAIN * backCenterAccum);
   servoCorrection = (PRESENT_GAIN * servoPresent) + (ACCUM_GAIN * servoAccum);
   heightScalarCorrection = (PRESENT_GAIN * heightScalarPresent) + (ACCUM_GAIN * heightScalarAccum);

   printf("Front right: %f \n", frontRightCorrection);
   printf("Front left: %f \n", frontLeftCorrection);
   printf("Back center: %f \n", backCenterCorrection);

   //Now set the motor percents with the corrections and height scaling applied
   pwm_control.setPercent(
           frontRightMotor,
           inRangeMotor((FRONT_RIGHT_PERCENT + frontRightCorrection) * (HEIGHT_SCALAR + heightScalarCorrection))
                          );
   pwm_control.setPercent(
           frontLeftMotor,
           inRangeMotor((FRONT_LEFT_PERCENT + frontLeftCorrection) * (HEIGHT_SCALAR + heightScalarCorrection))
                          );
   pwm_control.setPercent(
           backCenterMotor,
           inRangeMotor( (BACK_CENTER_PERCENT + backCenterCorrection) * (HEIGHT_SCALAR + heightScalarCorrection))
                          );

   //Servo doesn't get height scaling
   pwm_control.setPercent(backCenterServo, inRangeServo( SERVO_PERCENT + servoCorrection ));
}

bool MotorControlTask::violatesFailSafe()
{
    if (std::abs(orientation.x) > MAX_ANGLE)
    {
        printf("Killed due to orientation.x of %f \n", orientation.x);
        return true;
    }
    if (std::abs(orientation.y) > MAX_ANGLE)
    {
        printf("Killed due to orientation.y of %f \n", orientation.y);
        return true;
    }
    if (orientation.height > MAX_HEIGHT)
    {
        printf("Killed due to height of %f \n", orientation.height);
        return true;
    }

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


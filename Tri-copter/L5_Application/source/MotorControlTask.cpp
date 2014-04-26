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

    pwm_control.enablePort(frontLeftMotor);  //p2.0
    pwm_control.enablePort(frontRightMotor); //p2.1
    pwm_control.enablePort(backCenterMotor); //p2.2
    pwm_control.enablePort(backCenterServo); //p2.4

    heightScalar = 1;
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

    //Roll control
    //  Adjust the speed of either front motor
    //  axis x: right positive, left negative
    if (orientation.x > ZERO_X) //roll right => increase right motor
    {
        frontRightCorrection = SENSITIVITY_X * pow(fabs(orientation.x - ZERO_X) , CORRECTION_DEGREE);
    }
    else //roll left => increase left motor
    {
        frontLeftCorrection = SENSITIVITY_X * pow(fabs(orientation.x - ZERO_X), CORRECTION_DEGREE);
    }

    //Pitch control
    //  Increase/decrease speed of back motor
    //  axis y: up positive, down negative,
    if (orientation.y > ZERO_Y) //pitch up => decrease back motor
    {
        //note the negative
        backCenterCorrection = -(SENSITIVITY_Y * pow(fabs(orientation.y - ZERO_Y), CORRECTION_DEGREE));
    }
    else //pitch down => increase back motor
    {
        backCenterCorrection = SENSITIVITY_Y * pow(fabs(orientation.y - ZERO_Y), CORRECTION_DEGREE);
    }

    //Yaw control
    //  Increase or decrease servo
    //  axis z: counterclockwise positive, clockwise negative
    if (orientation.z > ZERO_Z) //yaw counter clockwisew => increase servo
    {
        servoCorrection = SENSITIVITY_Z * pow(fabs(orientation.z - ZERO_Z), CORRECTION_DEGREE);
    }
    else //yaw clockwisew => decrease servo
    {
        servoCorrection = SENSITIVITY_Z * pow(fabs(orientation.z - ZERO_Z), CORRECTION_DEGREE);
    }

   //Height control
   //  scale all percents by whether we are too low/high
   if (orientation.height < currentHeightTarget)
   {
       heightScalar += SENSITIVITY_HEIGHT;
   }
   else if (orientation.height > currentHeightTarget)
   {
       heightScalar -= SENSITIVITY_HEIGHT;
   }

   //Now set the motor percents with the corrections and height scaling applied
   pwm_control.setPercent(frontRightMotor,
                          inRangeMotor( (FRONT_RIGHT_PERCENT + frontRightCorrection) * heightScalar)
                          );
   pwm_control.setPercent(frontLeftMotor,
                          inRangeMotor( (FRONT_LEFT_PERCENT + frontLeftCorrection) * heightScalar)
                          );
   pwm_control.setPercent(backCenterMotor,
                          inRangeMotor( (BACK_CENTER_PERCENT + backCenterCorrection) * heightScalar)
                          );

   //Servo doesn't get height scaling
   pwm_control.setPercent(backCenterServo, inRangeServo(SERVO_PERCENT + servoCorrection));

   //Debug output, delete me
   /*printf("x: %f, y %f, z %f \n", orientation.x, orientation.y, orientation.z);
     printf("Front right motor: %f\n", std::min(PERCENT_MAX, frontRightMotorPercent * heightScalar));
     printf("Front left motor: %f\n", std::min(PERCENT_MAX, frontLeftMotorPercent * heightScalar));
     printf("Back center motor: %f\n", std::min(PERCENT_MAX, backCenterMotorPercent * heightScalar));
     printf("Back center servo: %f \n", backCenterServoPercent);
     printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); */
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


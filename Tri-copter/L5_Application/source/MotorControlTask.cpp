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

    pwm_control.enablePort(frontLeftMotor); //p2.0
    pwm_control.enablePort(frontRightMotor); //p2.1
    pwm_control.enablePort(backCenterMotor); //p2.2
    pwm_control.enablePort(backCenterServo); //p2.4

    //Initialize speeds and servo positions
    //TODO: is there an initial startup procedure for the ESCs?

    //Start: 43, 49, 58
    frontLeftMotorPercent = PERCENT_MIN + 30;
    frontRightMotorPercent = PERCENT_MIN + 35;
    backCenterMotorPercent = PERCENT_MIN + 55;
    backCenterServoPercent = 50;
    heightScalar = 1;
    currentHeightTarget = HOVER_HEIGHT_TARGET;

  /*  pwm_control.setPercent(frontLeftMotor, frontLeftMotorPercent);
    pwm_control.setPercent(frontRightMotor, frontRightMotorPercent);
    pwm_control.setPercent(backCenterMotor, backCenterMotorPercent);
    pwm_control.setPercent(backCenterServo, backCenterServoPercent); */

    pwm_control.setPercent(frontLeftMotor, 0);
    pwm_control.setPercent(frontRightMotor, 0);
    pwm_control.setPercent(backCenterMotor, 0);
    pwm_control.setPercent(backCenterServo, 50);

    return true;
}

void MotorControlTask::updateMotorServoControl()
{
    PWMController &pwm_control = PWMController::getInstance();
    //printf("x: %f, y %f, z %f \n", orientation.x, orientation.y, orientation.z);

    //Orientation roll (x): right positive, left negative
    //Orientation pitch (y): up positive, down negative,
    //Orientation yaw (z): ccw positive, cw negative

    //Roll control
    //  Adjust speed of appropriate front motor
    //  We have a choice of either decreasing one or increasing the other
    //  Try to be smart about it, and increase if we're below height target, decrease otherwise

    /*
    float rollIncAmt = (SENSITIVITY_X * fabs(orientation.x - ZERO_X));
    if (orientation.x > ZERO_X)
    {
        //If we've rolled right
        if (frontLeftMotorPercent > frontRightMotorPercent)
        {
            if (frontLeftMotorPercent - rollIncAmt >= PERCENT_MIN)
                frontLeftMotorPercent -= rollIncAmt;
        }
        else
        {
            if (frontRightMotorPercent + rollIncAmt <= PERCENT_MAX)
                frontRightMotorPercent += rollIncAmt;
        }
    }
    else if (orientation.x < ZERO_X)
    {
        if (frontRightMotorPercent > frontLeftMotorPercent)
        {
            if (frontRightMotorPercent - rollIncAmt >= PERCENT_MIN)
                frontRightMotorPercent -= rollIncAmt;
        }
        else
        {
            if (frontLeftMotorPercent + rollIncAmt <= PERCENT_MAX)
                frontLeftMotorPercent += rollIncAmt;
        }
    }

    //Pitch control
    //  Adjust speed of both front motors
    float pitchIncAmt = SENSITIVITY_Y * fabs(orientation.y - ZERO_Y);
    if (orientation.y > ZERO_Y)
    {

      //.  if (frontLeftMotorPercent + (pitchIncAmt) <= PERCENT_MAX)
      //      frontLeftMotorPercent += pitchIncAmt;
      //  if (frontRightMotorPercent + (pitchIncAmt) <= PERCENT_MAX)
      //      frontRightMotorPercent += pitchIncAmt;
        if (backCenterMotorPercent + pitchIncAmt <= PERCENT_MAX)
            backCenterMotorPercent += pitchIncAmt;
    }
    else if (orientation.y < ZERO_Y)
    {

      //  if (frontLeftMotorPercent - (pitchIncAmt) >= PERCENT_MAX)
      //      frontLeftMotorPercent -= pitchIncAmt;
       // if (frontRightMotorPercent - (pitchIncAmt) >= PERCENT_MAX)
       //     frontRightMotorPercent -= pitchIncAmt;
        if (backCenterMotorPercent - pitchIncAmt >= PERCENT_MIN)
            backCenterMotorPercent -= pitchIncAmt;
    }


    //Yaw control
    //  Adjust servo position, maybe adjust back motor speed?
   /* float yawIncAmt = SENSITIVITY_Z;
    if (orientation.z > ZERO_Z)
    {
        if (backCenterServoPercent - yawIncAmt > PERCENT_MIN_SERVO)
            backCenterServoPercent -= yawIncAmt;
    }
    else if (orientation.z < ZERO_Z)
    {
        if (backCenterServoPercent + yawIncAmt < PERCENT_MAX_SERVO)
            backCenterServoPercent += yawIncAmt;
    } */

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

   //REMOVE ME
   //heightScalar = 1;

   //For motor controls, make sure we don't exceed PERCENT_MAX after height scaling
   if (heightScalar > 1)
   {
       pwm_control.setPercent(frontRightMotor, std::min(PERCENT_MAX, frontRightMotorPercent * heightScalar));
       pwm_control.setPercent(frontLeftMotor, std::min(PERCENT_MAX, frontLeftMotorPercent * heightScalar));
       pwm_control.setPercent(backCenterMotor, std::min(PERCENT_MAX, backCenterMotorPercent * heightScalar));

      // printf("Front right motor: %f\n", std::min(PERCENT_MAX, frontRightMotorPercent * heightScalar));
      // printf("Front left motor: %f\n", std::min(PERCENT_MAX, frontLeftMotorPercent * heightScalar));
       //printf("Back center motor: %f\n", std::min(PERCENT_MAX, backCenterMotorPercent * heightScalar));

   }
   else if (heightScalar <= 1) //don't go below PERCENT_MIN after height scaling
   {
       pwm_control.setPercent(frontRightMotor, std::max(PERCENT_MIN, frontRightMotorPercent * heightScalar));
       pwm_control.setPercent(frontLeftMotor, std::max(PERCENT_MIN, frontLeftMotorPercent * heightScalar));
       pwm_control.setPercent(backCenterMotor, std::max(PERCENT_MIN, backCenterMotorPercent * heightScalar));

      // printf("Front right motor: %f\n", std::max(PERCENT_MIN, frontRightMotorPercent * heightScalar));
      // printf("Front left motor: %f\n", std::max(PERCENT_MIN, frontLeftMotorPercent * heightScalar));
      // printf("Back center motor: %f\n", std::max(PERCENT_MIN, backCenterMotorPercent * heightScalar));
   }

   pwm_control.setPercent(backCenterServo, backCenterServoPercent);
  // printf("Back center servo: %f \n", backCenterServoPercent);
  // printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

bool MotorControlTask::violatesFailSafe()
{
    if (std::abs(orientation.x) > MAX_X_ANGLE)
        return true;
    if (std::abs(orientation.y) > MAX_Y_ANGLE)
        return true;
  //  if (orientation.height > MAX_HEIGHT)
  //      return true;

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

void MotorControlTask::getOrientationFromQueue()
{
    //Fill orientation with data from the queue
    //  If there's no new data, we don't care and proceed with data
    //   from the last time, but this shouldn't happen
    xQueueReceive(orientation_queue, &orientation, 0);
}

bool MotorControlTask::run(void* p)
{
   // printf("m\n");
    tickCount++;

    getOrientationFromQueue();
    if ( !(paused = isNotReady() ) && !killed) //CANNOT enter this block if we've
    {                                          //killed it due to fail safe violation
        if ((!(killed = violatesFailSafe())))
        {
            if (isTimerExpired())
                currentHeightTarget = 0;
            updateMotorServoControl();
        }
        else
        {
            killMotors();
        }
    }

   // if (paused)
   //     printf("Paused, not ready\n");
    if (killed)
        printf("Killed, failsafe violation\n");
    return true;
}

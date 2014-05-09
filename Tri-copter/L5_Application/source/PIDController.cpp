/*
 * PIDController.cpp
 *
 *  Created on: May 2, 2014
 *      Author: sloretz
 */

#include "PIDController.hpp"

//written by looking at source from both these links
//https://github.com/diydrones/ardupilot/blob/6af705d4554defc27aa475dab99f918b26de3ce1/libraries/AC_PID/AC_PID.cpp
//https://github.com/osPID/osPID-Firmware/blob/master/osPID_Firmware/PID_v1.cpp

PIDController::PIDController(float _kp, float _ki, float _kd)
{
   kp = _kp;
   ki = _ki;
   kd = _kd;
   integrator = 0.0f;
   lastInput = 0.0f;
}

float PIDController::get_kp()
{
   return kp;
}

float PIDController::get_ki()
{
   return ki;
}

float PIDController::get_kd()
{
   return kd;
}

float PIDController::calculate_output(float input, float target, float dt)
{
   float error = target - input;

   float p = error;
   float i = integrator += error * dt;
   float d = (error - lastError) / dt;

   float output = (kp * p) + (ki * i) + (kd * d);

   lastError = error;
   return output;
}



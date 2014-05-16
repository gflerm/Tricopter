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

#define I_MAX 30

PIDController::PIDController(float _kp, float _ki, float _kd, float _ks)
{
   kp = _kp;
   ki = _ki;
   kd = _kd;
   ks = _ks;
   integrator = 0.0f;
   lastError = 0.0f;
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

float PIDController::get_ks()
{
   return ks;
}

void PIDController::updateConstants(float _kp, float _ki, float _kd, float _ks)
{
    kp = _kp;
    ki = _ki;
    kd = _kd;
    ks = _ks;
    integrator = 0;
}

float PIDController::calculate_output(float actual, float target, float dt)
{
   float error = target - actual;

   float p = error;
   integrator += error * dt;
   if (integrator * ks > I_MAX)
       integrator = I_MAX / ks;
   if (integrator * ks < -I_MAX)
       integrator = -I_MAX / ks;
   float i = integrator;
   float d = (error - lastError) / dt;

   float output = (kp * p) + (ki * i) + (kd * d);

   lastError = error;
   return ks * output;
}



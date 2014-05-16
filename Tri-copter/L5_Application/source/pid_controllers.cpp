/*
 * pid_controllers.cpp
 *
 *  Created on: May 12, 2014
 *      Author: sloretz
 */

#include "pid_controllers.hpp"


//Sensitivity settings
static const float SENSITIVITY_X = 250; //scalar for how fast the motors should spin up
static const float SENSITIVITY_Y = 0;
static const float SENSITIVITY_Z = 150; //servo
static const float SENSITIVITY_HEIGHT = 2.5; //percent

//PID controller constants
static const float ROLL_KP = 0.6f;//.55
static const float ROLL_KI = 0.35f;
static const float ROLL_KD = 0.05f;

static const float PITCH_KP = 0.6f;
static const float PITCH_KI = 0.35f;
static const float PITCH_KD = 0.05f;

static const float YAW_KP = 0.5f;
static const float YAW_KI = 0.45f;
static const float YAW_KD = 0.05f;

static const float HEIGHT_KP = 0.6f;
static const float HEIGHT_KI = 0.35f;
static const float HEIGHT_KD = 0.05f;

static const float ROLL_INIT_I = 0;
static const float PITCH_INIT_I = 0;
static const float YAW_INIT_I = 0;
static const float HEIGHT_INIT_I = 0;

PIDController pid_roll(ROLL_KP, ROLL_KI, ROLL_INIT_I, ROLL_KD, SENSITIVITY_X);
PIDController pid_pitch(PITCH_KP, PITCH_KI, PITCH_INIT_I, PITCH_KD, SENSITIVITY_Y);
PIDController pid_yaw(YAW_KP, YAW_KI, YAW_INIT_I, YAW_KD, SENSITIVITY_Z);
PIDController pid_height(HEIGHT_KP, HEIGHT_KI, HEIGHT_INIT_I, HEIGHT_KD, SENSITIVITY_HEIGHT);



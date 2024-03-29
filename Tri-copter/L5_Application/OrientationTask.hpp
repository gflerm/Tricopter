/*
 * Orientation.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#ifndef ORIENTATION_HPP_
#define ORIENTATION_HPP_

#include "10dof.hpp"
#include "scheduler_task.hpp"
#include <cmath> //abs() and atan2()
#include "eint.h"
#include "utilities.h"
using namespace _10dof;

//Computes the orientation of the tricopter from a combination of the gyro and accelerometer.
//Makes some assumptions:
//  a. Z axis = vertical
//  b. Gyroscope output: millidegrees/second
class OrientationTask : public scheduler_task
{
public:
     //Orientation()
     //Supplies some defaults to scheduler_task constructor
     OrientationTask() : scheduler_task("orientation_task", STACK_SIZE_BYTES, PRIORITY_HIGH, NULL) { };

     //init()
     //Initializes the accel and gyro sensors
     bool init();

     //run()
     //Calculates the orientation. Is called automatically by the scheduler
     //Idea from:
     //NOTE: For this function to work properly, it needs to be called every
     //      ORIENTATION_UDPATE_TIME ms; this is accomplished with a call to
     //      scheduler_task::setRunDuration() in init()
     bool run(void* p);

     //get_orientation()
     //Returns a pointer to the orientation of the tricopter, in radians
     //For debug only
     orientation_t* get_orientation();

     //get_queue_handle
     //Returns the orientation queue handle
     QueueHandle_t get_queue_handle()
     {
         return orientation_queue;
     }

     three_axis_info_t get_raw_accel()
     {
         return accel_sensor->getXYZ();
     }
     three_axis_info_t get_raw_gyro()
     {
         return gyro_sensor->getXYZ();
     }

     //get_height()
     //Returns the height, in inches
     float get_height();

private:
     //Stores the calculated orientation
     QueueHandle_t orientation_queue;
     orientation_t orientation;

     Accelerometer* accel_sensor;
     Gyroscope* gyro_sensor;

     //Use this to filter out garbage from ultrasonic sensor data readings
     static const float APPROX_INIT_HEIGHT = 2; //lastHeight will be initialized to this value
     static const float HEIGHT_CHANGE_THRESHOLD = 5; //inches

     //Task control settings
     static const int STACK_SIZE_BYTES = 4096;
     static const float ORIENTATION_UPDATE_TIME =  5; //ms, want to run this task as often as possible

     //Calibration settings for the complementary filter
     static const float FILTER_PERCENT_HIGH = .98;
     static const float FILTER_PERCENT_LOW = .02;

     //Gyro noise filter
     static const float GYRO_NOISE_FLOOR = 125;

     //These should be set to filter out any accelerometer weirdness
     //Add all axes together to find the total magnitude
     static const uint16_t ACCEL_MAGNITUDE_LOW = 256;
     static const uint16_t ACCEL_MAGNITUDE_HIGH = 4096;

     //constants
     static const float GRAVITY_ACCEL = 9.81;
     static const double PI = 3.1415926;

     //Converts data received by accel sensor to something more usable
     //   -> 4mg per bit to meters per second squared
     inline float toMetersPerSecondSq(int mg)
     {
         return mg * GRAVITY_ACCEL * 4 / 1000;
     }

     //Converts data received by gyroscope to something more usuable
     //  -> milli degrees per second to radians per second
     inline float toRadians(float millidegrees)
     {
         return (millidegrees / 1000.0) / 180 * PI;
     }

     //Converts from milliseconds to seconds
     inline float toSeconds(int milliseconds)
     {
         return milliseconds / 1000.0;
     }

     bool isValidUltrasonicReading(float currentHeight, float previousHeight)
     {
         if (fabs(currentHeight - previousHeight) < HEIGHT_CHANGE_THRESHOLD)
         {
             return true;
         }
         return false;
     }
};

#endif /* ORIENTATION_HPP_ */

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
     bool run(void* p);

     //get_orientation()
     //Returns the orientation of the tricopter, in radians
     //NOTE: For this function to work properly, it needs to be called every
     //      ORIENTATION_UDPATE_TIME ms; this is accomplished with a call to
     //      scheduler_task::setRunDuration() in init()
     three_axis_info_t get_orientation();
private:
     three_axis_info_t orientation;
     Accelerometer* accel_sensor;
     Gyroscope* gyro_sensor;

     //Task control settings
     static const int STACK_SIZE_BYTES = 4096;
     static const float ORIENTATION_UPDATE_TIME =  5; //ms, want to run this task as often as possible

     //Calibration settings for the complementary filter
     static const float FILTER_PERCENT_HIGH = .98;
     static const float FILTER_PERCENT_LOW = .02;

     //These should be set depending upon the output range of the accelerometer
     //i.e., if the output range is 10 bits and the g-range is +-2g then a reasonable
     //value would be between ~640 (.5g on only one axis) to ~2304 (3g over all axes)
     static const uint16_t ACCEL_MAGNITUDE_LOW = 640;
     static const uint16_t ACCEL_MAGNITUDE_HIGH = 2304;

     three_axis_info_t accel_data;
     three_axis_info_t gyro_data;
     three_axis_info_t accel_calc;

     uint16_t accel_magnitude;

     //Converts from milliseconds to seconds
     inline float toSeconds(int milliseconds)
     {
         return milliseconds / 1000.0;
     }

     //Converts from milli degrees to radians
     const double PI = 3.1415926;
     inline float toRadians(float degrees)
     {
         return (degrees / 1000.0) / 180 * PI;
     }
};

#endif /* ORIENTATION_HPP_ */

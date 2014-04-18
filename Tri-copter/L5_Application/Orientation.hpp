/*
 * Orientation.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#ifndef ORIENTATION_HPP_
#define ORIENTATION_HPP_

#include "10dof.hpp"
#include "tasks.hpp"
#include <cmath>

using namespace _10dof;

namespace _OrientationTask
{

//Computes the orientation of the tricopter from a combination of the gyro and accelerometer.
class OrientationTask : public scheduler_task
{
public:
     //Orientation()
     //Supplies some defaults to scheduler_task constructor
     OrientationTask() : scheduler_task("orientation_task", 4096, PRIORITY_HIGH, 0) { };

     //init()
     //Initializes the accel and gyro sensors
     bool init();

     //run()
     //Calculates the orientation. Is called automatically by the scheduler
     bool run(void* p);

     //get_orientation()
     //Returns the orientation of the tricopter, in radians
     three_axis_info_t get_orientation();
private:
     three_axis_info_t orientation;
     Accelerometer* accel_sensor;
     Gyroscope* gyro_sensor;

     static const float ORIENTATION_UPDATE_TIME =  20 / 100000; //ms converted to seconds
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
};



}




#endif /* ORIENTATION_HPP_ */

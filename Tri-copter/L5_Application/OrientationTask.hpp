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

#define AVERAGE_AMOUNT 4            //Adjust to change average filtering amount

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

     three_axis_info_t accel_data;
     three_axis_info_t gyro_data;
     orientation_t accel_calc;

     uint16_t accel_magnitude;

     //Count for average filtering
     uint8_t count;

     //Average array for orientation
     orientation_t orientation_avg[4];

     //Compute average for filtering
     void compute_average();


     //Use this to filter out garbage from ultrasonic sensor data readings
     float lastHeight;
     static const float APPROX_INIT_HEIGHT = 2; //lastHeight will be initialized to this value
     static const float HEIGHT_CHANGE_THRESHOLD = 3; //inches

     //Task control settings
     static const int STACK_SIZE_BYTES = 4096;
     static const float ORIENTATION_UPDATE_TIME =  5; //ms, want to run this task as often as possible

     //Calibration settings for the complementary filter
     static const float FILTER_PERCENT_HIGH = .98;
     static const float FILTER_PERCENT_LOW = .02;

     //Gyro noise filter
     static const float GYRO_NOISE_FLOOR = 0;

     //These should be set depending upon the output range of the accelerometer
     //i.e., if the output range is 10 bits and the g-range is +-2g then a reasonable
     //value would be between ~640 (.5g on only one axis) to ~2304 (3g over all axes)
     static const uint16_t ACCEL_MAGNITUDE_LOW = 125;
     static const uint16_t ACCEL_MAGNITUDE_HIGH = 500;

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

     bool isValidUltrasonicReading(float height)
     {
         if (fabs(height - lastHeight) < HEIGHT_CHANGE_THRESHOLD)
         {
             lastHeight = height;
             return true;
         }
         else
         {
             return false;
         }
     }
};

#endif /* ORIENTATION_HPP_ */

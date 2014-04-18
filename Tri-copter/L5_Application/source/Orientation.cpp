/*
 * Orientation.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "Orientation.hpp"

using namespace _10dof;
using namespace _OrientationTask;

bool OrientationTask::init()
{
    accel_sensor = &(Accelerometer::getInstance());
    gyro_sensor = &(Gyroscope::getInstance());

    setRunDuration(ORIENTATION_UPDATE_TIME);

    if (accel_sensor->init() && gyro_sensor->init())
        return true;
    else
        return false;
}

three_axis_info_t OrientationTask::get_orientation()
{
    return orientation;
}

bool OrientationTask::run(void* p)
{
    //Grab info from accelerometer and gyroscope
    accel_data = accel_sensor->getXYZ();
    gyro_data = gyro_sensor->getXYZ();

    //Integrate gyroscope data
    orientation.x.word += (gyro_data.x.word) * ORIENTATION_UPDATE_TIME;
    orientation.y.word += (gyro_data.y.word) * ORIENTATION_UPDATE_TIME;
    orientation.z.word += (gyro_data.z.word) * ORIENTATION_UPDATE_TIME;

    //Find the angle from the acceleration data
    accel_calc.x.word = atan2(accel_data.z.word, accel_data.x.word);
    accel_calc.y.word = atan2(accel_data.z.word, accel_data.y.word);

    //If we can trust the accelerometer data, then add it in
    accel_magnitude = std::abs(accel_data.x.word) + std::abs(accel_data.y.word) + std::abs(accel_data.z.word);
    if ( accel_magnitude > ACCEL_MAGNITUDE_LOW && accel_magnitude < ACCEL_MAGNITUDE_HIGH)
    {
        //Complementary filter
        //High pass filter the gyro data, low pass filter the acceleration data
        orientation.x.word = (FILTER_PERCENT_HIGH * (orientation.x.word + gyro_data.x.word * ORIENTATION_UPDATE_TIME)) +
                             (FILTER_PERCENT_LOW  * accel_calc.x.word);
        orientation.y.word = (FILTER_PERCENT_HIGH * (orientation.y.word + gyro_data.y.word * ORIENTATION_UPDATE_TIME)) +
                             (FILTER_PERCENT_LOW * accel_calc.y.word);
        //orientation.z = <<TODO>> use magnetometer if this drifts
    }

    //Delay for a fixed period so the integral code works out
    //vTaskDelayUntil(&startTime, (ORIENTATION_UPDATE_TIME * 1000 / portTICK_PERIOD_MS));

    return true; //should never get here
}

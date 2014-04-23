/*
 * Orientation.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "OrientationTask.hpp"

using namespace _10dof;

bool OrientationTask::init()
{
    accel_sensor = &(Accelerometer::getInstance());
    gyro_sensor = &(Gyroscope::getInstance());

    velocity_vertical = 0;
    height = 0;

    setRunDuration(ORIENTATION_UPDATE_TIME);

    if (accel_sensor->init() && gyro_sensor->init())
        return true;
    else
        return false;
}

three_axis_info_t* OrientationTask::get_orientation()
{
    return &orientation;
}

float* OrientationTask::get_height()
{
    return &height;
}

bool OrientationTask::run(void* p)
{
    float secondsSinceLastUpdate = toSeconds(ORIENTATION_UPDATE_TIME);

    //Grab info from accelerometer and gyroscope
    accel_data = accel_sensor->getXYZ();
    gyro_data = gyro_sensor->getXYZ();


    //Double integrate accelerometer data for the height
    velocity_vertical += toMetersPerSecondSq(accel_data.z.word) * secondsSinceLastUpdate;
    height += velocity_vertical * secondsSinceLastUpdate;

    //Integrate gyroscope data
    orientation.x.word += toRadians((gyro_data.x.word)) * secondsSinceLastUpdate;
    orientation.y.word += toRadians((gyro_data.y.word)) * secondsSinceLastUpdate;

    //Find the angle in radians from the acceleration data
    accel_calc.x.word = atan2(accel_data.z.word, accel_data.x.word);
    accel_calc.y.word = atan2(accel_data.z.word, accel_data.y.word);

    //If we can trust the accelerometer data, then add it in
    accel_magnitude = std::abs(accel_data.x.word) + std::abs(accel_data.y.word) + std::abs(accel_data.z.word);
    if ( accel_magnitude > ACCEL_MAGNITUDE_LOW && accel_magnitude < ACCEL_MAGNITUDE_HIGH)
    {
        //Complementary filter
        //High pass filter the gyro data, low pass filter the acceleration data
        orientation.x.word = (FILTER_PERCENT_HIGH * (orientation.x.word + toRadians(gyro_data.x.word) * secondsSinceLastUpdate)) +
                             (FILTER_PERCENT_LOW  * accel_calc.x.word);
        orientation.y.word = (FILTER_PERCENT_HIGH * (orientation.y.word + toRadians(gyro_data.y.word) * secondsSinceLastUpdate)) +
                             (FILTER_PERCENT_LOW * accel_calc.y.word);
    }

    return true;
}

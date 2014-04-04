#include "10dof.hpp"

namespace _10dof
{

axis_info_t Accelerometer::getX()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x32, reading.byte,2);
    return reading;
}

axis_info_t Accelerometer::getY()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x34, reading.byte,2);
    return reading;
}

axis_info_t Accelerometer::getZ()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x36, reading.byte,2);
    return reading;
}

three_axis_info_t Accelerometer::getXYZ()
{
    three_axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x32, reading.byte,6);
    return reading;
}

void Accelerometer::callibrate()
{
    //Get average of 10 readings
    //write average to offset register
}

//TODO everything below


axis_info_t Gyroscope::getX()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x28, reading.byte,2);
    return reading;
}

axis_info_t Gyroscope::getY()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x2A, reading.byte,2);
    return reading;
}

axis_info_t Gyroscope::getZ()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x2c, reading.byte,2);
    return reading;
}

three_axis_info_t Gyroscope::getXYZ()
{
    three_axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x28, reading.byte,6);
    return reading;
}

void Gyroscope::calibrate()
{
    //todo?
}

axis_info_t Magnometer::getX()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x03, reading.byte,2);
    return reading;
}

axis_info_t Magnometer::getY()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x07, reading.byte,2);
    return reading;
}

axis_info_t Magnometer::getZ()
{
    axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x05, reading.byte,2);
    return reading;
}

three_axis_info_t Magnometer::getXYZ()
{
    three_axis_info_t reading;
    mI2C.readRegisters(mOurAddr, 0x32, reading.byte,6);
    //swap Z and y
    axis_info_t swap = reading.y;
    reading.y = reading.z;
    reading.z = swap;
    return reading;
}

void Magnometer::calibrate()
{
    //todo?
}

}

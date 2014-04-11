#include "10dof.hpp"

namespace _10dof
{

axis_info_t Accelerometer::getX()
{
    axis_info_t reading;
    readRegisters(0x32, reading.byte,2);
    return reading;
}

axis_info_t Accelerometer::getY()
{
    axis_info_t reading;
    readRegisters(0x34, reading.byte,2);
    return reading;
}

axis_info_t Accelerometer::getZ()
{
    axis_info_t reading;
    readRegisters(0x36, reading.byte,2);
    return reading;
}

three_axis_info_t Accelerometer::getXYZ()
{
    three_axis_info_t reading;
    readRegisters(0x32, reading.byte,6);
    return reading;
}

void Accelerometer::callibrate()
{
    //Get average of 10 readings
    //write average to offset register
    int16_t x = 0, y = 0, z = 0;
    char axi[3];

    for(int i=0; i<10; i++)
    {
        three_axis_info_t reading = getXYZ();
        x += reading.x.byte[0];
        y += reading.y.byte[0];
        z += reading.z.byte[0];
    }

    //Average and negate
    axi[0] = x / -10;
    axi[1] = y / -10;
    axi[2] = z / -10;

    writeRegisters(0x1E, axi, 3); //Write to offset register
}

//TODO everything below


axis_info_t Gyroscope::getX()
{
    axis_info_t reading;
    readRegisters(0x28, reading.byte,2);
    return reading;
}

axis_info_t Gyroscope::getY()
{
    axis_info_t reading;
    readRegisters(0x2A, reading.byte,2);
    return reading;
}

axis_info_t Gyroscope::getZ()
{
    axis_info_t reading;
    readRegisters(0x2c, reading.byte,2);
    return reading;
}

three_axis_info_t Gyroscope::getXYZ()
{
    three_axis_info_t reading;
    readRegisters(0x28, reading.byte,6);
    return reading;
}

void Gyroscope::calibrate()
{
    //todo?
}

bool Magnometer::init()
{
    writeRegisters(0x10, (char*)&configReg_A, 1);
    writeRegisters(0x20, (char*)&configReg_B, 1);
    return true;
}

axis_info_t Magnometer::getX()
{
    axis_info_t reading;
    readRegisters(0x03, reading.byte,2);
    return reading;
}

axis_info_t Magnometer::getY()
{
    axis_info_t reading;
    readRegisters(0x07, reading.byte,2);
    return reading;
}

axis_info_t Magnometer::getZ()
{
    axis_info_t reading;
    readRegisters(0x05, reading.byte,2);
    return reading;
}

three_axis_info_t Magnometer::getXYZ()
{
    three_axis_info_t reading;
    readRegisters(0x32, reading.byte,6);
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

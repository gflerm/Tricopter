#include "10dof.hpp"


// Defines taken from
//https://github.com/pololu/l3g4200d-arduino/blob/master/L3G4200D/L3G4200D.h
#define L3G4200D_WHO_AM_I 0x0F

#define L3G4200D_CTRL_REG1 0x20
#define L3G4200D_CTRL_REG2 0x21
#define L3G4200D_CTRL_REG3 0x22
#define L3G4200D_CTRL_REG4 0x23
#define L3G4200D_CTRL_REG5 0x24
#define L3G4200D_REFERENCE 0x25
#define L3G4200D_OUT_TEMP 0x26
#define L3G4200D_STATUS_REG 0x27

#define L3G4200D_OUT_X_L 0x28
#define L3G4200D_OUT_X_H 0x29
#define L3G4200D_OUT_Y_L 0x2A
#define L3G4200D_OUT_Y_H 0x2B
#define L3G4200D_OUT_Z_L 0x2C
#define L3G4200D_OUT_Z_H 0x2D

#define L3G4200D_FIFO_CTRL_REG 0x2E
#define L3G4200D_FIFO_SRC_REG 0x2F

#define L3G4200D_INT1_CFG 0x30
#define L3G4200D_INT1_SRC 0x31
#define L3G4200D_INT1_THS_XH 0x32
#define L3G4200D_INT1_THS_XL 0x33
#define L3G4200D_INT1_THS_YH 0x34
#define L3G4200D_INT1_THS_YL 0x35
#define L3G4200D_INT1_THS_ZH 0x36
#define L3G4200D_INT1_THS_ZL 0x37
#define L3G4200D_INT1_DURATION 0x38

namespace _10dof
{

bool Accelerometer::init()
{
    writeReg(0x2D, 0x08); // Accelerometer power
    writeReg(0x31, 0x08);
    return true;
}

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

void Accelerometer::calibrate()
{
    //Get average of 10 readings
    //write average to offset register
    int16_t x = 0, y = 0, z = 0;
    char axi[3] = {0,0,0};
    //Clear offset register
    writeRegisters(0x1E, axi, 3);

    for(int i=0; i<10; i++)
    {
        vTaskDelay(10);
        three_axis_info_t reading = getXYZ();
        x += reading.x.word;
        y += reading.y.word;
        z += reading.z.word;
    }

    //Average and negate
    axi[0] = x / -10;
    axi[1] = y / -10;
    axi[2] = z / -10;

    //Write to offset register
    //writeRegisters(0x1E, axi, 3);
}


bool Gyroscope::init()
{
    //Use bypass mode (default)
    //Enable all axes and power on
    writeReg(L3G4200D_CTRL_REG1, 0x0F);
    return true;
}

axis_info_t Gyroscope::getX()
{
    axis_info_t reading;
    readRegisters(L3G4200D_OUT_X_L, reading.byte,1);
    readRegisters(L3G4200D_OUT_X_H, reading.byte+1,1);
    return reading;
}

axis_info_t Gyroscope::getY()
{
    axis_info_t reading;
    readRegisters(L3G4200D_OUT_Y_L, reading.byte,1);
    readRegisters(L3G4200D_OUT_Y_H, reading.byte+1,1);
    return reading;
}

axis_info_t Gyroscope::getZ()
{
    axis_info_t reading;
    readRegisters(L3G4200D_OUT_Z_L, reading.byte,1);
    readRegisters(L3G4200D_OUT_Z_H, reading.byte+1,1);
    return reading;
}

three_axis_info_t Gyroscope::getXYZ()
{
    three_axis_info_t reading;
    readRegisters(L3G4200D_OUT_X_L, reading.x.byte,1);
    readRegisters(L3G4200D_OUT_X_H, reading.x.byte+1,1);
    readRegisters(L3G4200D_OUT_Y_L, reading.y.byte,1);
    readRegisters(L3G4200D_OUT_Y_H, reading.y.byte+1,1);
    readRegisters(L3G4200D_OUT_Z_L, reading.z.byte,1);
    readRegisters(L3G4200D_OUT_Z_H, reading.z.byte+1,1);
    return reading;
}

char Gyroscope::whoami()
{
    char id = 0;
    readRegisters(L3G4200D_WHO_AM_I, &id,1);
    return id;
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

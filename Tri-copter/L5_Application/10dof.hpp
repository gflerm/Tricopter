/*
 * 10dof sensor interface classes
 * Make use of existing I2C_Device_Base class
 */

#ifndef _10_dof_hpp
#define _10_dof_hpp

#include "src/I2C_Device_Base.hpp"
#include <stdint.h>



namespace _10dof
{

const uint8_t c_accel_addr = 0xA7;
const uint8_t c_gyro_addr = 0xD3;
const uint8_t c_magno_addr = 0x3D; //3D -> read, 3C -> write

typedef union{
    char byte[2];
    int16_t word;
}axis_info_t;

typedef union {
        char byte[6];
        struct{
        axis_info_t x,y,z;
        };
}three_axis_info_t;

typedef struct{
        float x;
        float y;
        float z;
} orientation_t;

class Accelerometer : protected I2C_Device_Base, public SingletonTemplate<Accelerometer>
{
    public:
        bool init();   ///< Initializes the sensor

        axis_info_t getX();  ///< @returns X-Axis value
        axis_info_t getY();  ///< @returns Y-Axis value
        axis_info_t getZ();  ///< @returns Z-Axis value
        three_axis_info_t getXYZ(); //returns 3 values at once
        void calibrate();

    private:
        /// Private constructor of this Singleton class
        Accelerometer() : I2C_Device_Base(c_accel_addr)
        {
        }
        friend class SingletonTemplate<Accelerometer>;  ///< Friend class used for Singleton Template
};

class Gyroscope : protected I2C_Device_Base, public SingletonTemplate<Gyroscope>
{
    public:
        bool init();   ///< Initializes the sensor

        axis_info_t getX();  ///< @returns X-Axis value
        axis_info_t getY();  ///< @returns Y-Axis value
        axis_info_t getZ();  ///< @returns Z-Axis value
        three_axis_info_t getXYZ(); //returns 3 values at once
        char whoami();
        void calibrate();

    private:
        /// Private constructor of this Singleton class
        Gyroscope() : I2C_Device_Base(c_gyro_addr)
        {
        }
        friend class SingletonTemplate<Gyroscope>;  ///< Friend class used for Singleton Template
};

class Magnometer : protected I2C_Device_Base, public SingletonTemplate<Magnometer>
{
    public:
        bool init();   ///< Initializes the sensor

        axis_info_t getX();  ///< @returns X-Axis value
        axis_info_t getY();  ///< @returns Y-Axis value
        axis_info_t getZ();  ///< @returns Z-Axis value
        three_axis_info_t getXYZ(); //returns 3 values at once
        void calibrate();

    private:
        //Config Reg A
        //[7:7] unused
        //[6:5] number of samples to average
        //[4:2] data rate
        //[1:0] measurement config
        static const uint8_t configReg_A = 0x78;

        //Config Reg B
        //[7:5] gain
        //[4:0] must be cleared
        static const uint8_t configReg_B = 0x00;

        /// Private constructor of this Singleton class
        Magnometer() : I2C_Device_Base(c_magno_addr)
        {
        }
        friend class SingletonTemplate<Magnometer>;  ///< Friend class used for Singleton Template
};

}

#endif

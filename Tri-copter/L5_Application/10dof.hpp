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
    uint16_t word;
}axis_info_t;

typedef union {
        char byte[6];
        struct{
        axis_info_t x,y,z;
        };
}three_axis_info_t;

class Accelerometer : private I2C_Device_Base, public SingletonTemplate<Acceleration_Sensor>
{
    public:
        bool init();   ///< Initializes the sensor

        axis_info_t getX();  ///< @returns X-Axis value
        axis_info_t getY();  ///< @returns Y-Axis value
        axis_info_t getZ();  ///< @returns Z-Axis value
        three_axis_info_t getXYZ(); //returns 3 values at once
        void callibrate();

    private:
        /// Private constructor of this Singleton class
        Accelerometer() : I2C_Device_Base(c_accel_addr)
        {
        }
        friend class SingletonTemplate<Acceleration_Sensor>;  ///< Friend class used for Singleton Template
};

class Gyroscope : private I2C_Device_Base, public SingletonTemplate<Acceleration_Sensor>
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
        Gyroscope() : I2C_Device_Base(c_gyro_addr)
        {
        }
        friend class SingletonTemplate<Acceleration_Sensor>;  ///< Friend class used for Singleton Template
};

class Magnometer : private I2C_Device_Base, public SingletonTemplate<Acceleration_Sensor>
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
        Magnometer() : I2C_Device_Base(c_magno_addr)
        {
        }
        friend class SingletonTemplate<Acceleration_Sensor>;  ///< Friend class used for Singleton Template
};

}

#endif

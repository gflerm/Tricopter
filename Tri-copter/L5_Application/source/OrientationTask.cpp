/*
 * Orientation.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Michael
 */

#include "OrientationTask.hpp"

using namespace _10dof;

#include <cstdio>

#define DIST_PIN 30

static float total; // made global to simplify life.

void distance_sensor(){ // Interrupt Handler
    if( LPC_GPIOINT->IO0IntStatR &(1<<DIST_PIN)){
        LPC_TIM3->TCR &= (1<<0); //enable pin
        LPC_GPIOINT->IO0IntClr |= (1<<DIST_PIN); // clear interrupt
        LPC_GPIOINT->IO0IntEnR &= (1<<DIST_PIN); //disable rising edge
        LPC_GPIOINT->IO0IntEnF |= (1<<DIST_PIN); //enable falling edge
    }
    else if(LPC_GPIOINT->IO0IntStatF & (1<<DIST_PIN)){
        total = LPC_TIM3->TC; // Capture time info
        LPC_GPIOINT->IO0IntClr |= (1<<DIST_PIN); // clear interrupt
        LPC_TIM3->TCR |=(1<<0); // disable timer
        LPC_TIM3->TCR |= (1<<1); // RESET TO ZERO
        LPC_GPIOINT->IO0IntEnF &= (1<<DIST_PIN); //disable falling edge
        LPC_GPIOINT->IO0IntEnR |= (1<<DIST_PIN); //enable rising edge
    }
}

bool OrientationTask::init()
{
    setRunDuration(ORIENTATION_UPDATE_TIME);

    orientation.x = 0;
    orientation.y = 0;
    orientation.z = 0;
    orientation.height = 0;

    orientation_queue = xQueueCreate(1, (sizeof(orientation_t)));

    //Height Sensor
    LPC_SC->PCONP |= (1<<23); // enable hardware timer 3
    LPC_SC->PCLKSEL1 &=(1<<14); // USE CCLK
    LPC_TIM3->TCR |= (1<<0); // DISABLE TIMER
    LPC_TIM3->TCR |= (1<<1); // RESET TO ZERO
    LPC_PINCON->PINSEL3 &= (3<<6); //SET pin 1.19 to gpio
    LPC_GPIO1->FIODIR |= (1<<19); //SET PIN 1.19 to output
    eint3_enable_port0(DIST_PIN, eint_rising_edge, *distance_sensor);

    lastHeight = APPROX_INIT_HEIGHT;

    accel_sensor = &(Accelerometer::getInstance());
    gyro_sensor = &(Gyroscope::getInstance());

    if (accel_sensor->init() && gyro_sensor->init())
        return true;
    else
        return false;
}

orientation_t* OrientationTask::get_orientation()
{
    return &orientation;
}

float OrientationTask::get_height()
{
    return orientation.height;
}

bool OrientationTask::run(void* p)
{
   // printf("o\n");
    float secondsSinceLastUpdate = toSeconds(ORIENTATION_UPDATE_TIME);

    //Grab height info from Ultra sonic sensor (needs around 500us after going low)
    LPC_GPIO1->FIOSET |= (1<<19); // set high
    delay_us(10);
    LPC_GPIO1->FIOCLR |= (1<<19); // set low

    //Grab info from accelerometer and gyroscope
    accel_data = accel_sensor->getXYZ();
    gyro_data = gyro_sensor->getXYZ();

    orientation.gx = gyro_data.x.word;
    orientation.gy = gyro_data.y.word;
    orientation.gz = gyro_data.z.word;

    orientation.ax = (accel_data.x.word << 4);
    orientation.ay = (accel_data.y.word << 4);
    orientation.az = (accel_data.z.word << 4);

    //Integrate gyroscope data
    orientation.x += toRadians((gyro_data.x.word)) * secondsSinceLastUpdate;
    orientation.y += toRadians((gyro_data.y.word)) * secondsSinceLastUpdate;
    if (std::abs(gyro_data.z.word) > GYRO_NOISE_FLOOR)
        orientation.z += toRadians((gyro_data.z.word)) * secondsSinceLastUpdate;

    //Calculate height in inches
    //orientation.height =(.120*total)/148; // should be .208 but .120 provides better result
    float read_height = (.120 * total) / 148;
    if (isValidUltrasonicReading(read_height))
        orientation.height = read_height;

    //~~~~~~~~~~NOTE: ACCELEROMETER AXES ARE OPPOSITE OF GYROSCOPE~~~~~~~~~~~~
    //Find the angle in radians from the acceleration data
    accel_calc.x = atan2(accel_data.y.word, accel_data.z.word);
    accel_calc.y = atan2(accel_data.x.word, accel_data.z.word);

    //If we can trust the accelerometer data, then add it in
    accel_magnitude = std::abs(accel_data.x.word) + std::abs(accel_data.y.word) + std::abs(accel_data.z.word);
   if ( accel_magnitude > ACCEL_MAGNITUDE_LOW && accel_magnitude < ACCEL_MAGNITUDE_HIGH)
   {
        //Complementary filter
        //High pass filter the gyro data, low pass filter the acceleration data
        orientation.x = (FILTER_PERCENT_HIGH * (orientation.x + toRadians(gyro_data.x.word) * secondsSinceLastUpdate)) +
                             (FILTER_PERCENT_LOW  * accel_calc.x);
        orientation.y = (FILTER_PERCENT_HIGH * (orientation.y + toRadians(gyro_data.y.word) * secondsSinceLastUpdate)) +
                             (FILTER_PERCENT_LOW * accel_calc.y);
   }

    //Place calculated orientation in queue for motor control to receive, if there's space
    //If there's not, we don't care and we don't wait
    xQueueSend(orientation_queue, &orientation, 0);

    return true;
}

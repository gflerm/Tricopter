/*
 * ServoTask.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: sloretz
 */

#ifndef SERVOTASK_HPP_
#define SERVOTASK_HPP_
#include <stdint.h>

/*
 * A servo class that allows a new pulse width value
 * to be put to use immediately
 */

class ServoController
{
    public:
        typedef enum {
            pwm1=0, ///< P2.0
            pwm2=1, ///< P2.1
            pwm3=2, ///< P2.2
            pwm4=3, ///< P2.0
            pwm5=4, ///< P2.1
            pwm6=5  ///< P2.2
        } pwmType;

        ServoController();


        //Begin PWM output on this port
        bool enablePort(pwmType port);

        //Sets the next orientation of the servo
        //value between -1.0f and 1.0f
        void setNextPosition(pwmType port, float orientation);

    private:
        uint32_t enableMask;
};

#endif /* SERVOTASK_HPP_ */

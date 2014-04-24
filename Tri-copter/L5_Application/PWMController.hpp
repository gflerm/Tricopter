/*
 * ServoTask.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: sloretz
 */

#ifndef PWMCONTROLLER_HPP_
#define PWMCONTROLLER_HPP_
#include <stdint.h>
#include "lpc_sys.h"

/*
 * A servo class that allows a new pulse width value
 * to be put to use immediately
 */

class PWMController
{
    public:
        typedef enum {
            pwm0=0, ///< P2.0
            pwm1=1, ///< P2.1
            pwm2=2, ///< P2.2
            pwm3=3, ///< P2.3
            pwm4=4, ///< P2.4
            pwm5=5  ///< P2.5
        } pwmType;

        PWMController();

        //Begin PWM output on this port
        bool enablePort(pwmType port);

        //Set PWM percent
        virtual void setPercent(pwmType port, float percent) = 0;

        //Sets the next orientation of the servo
        //value between -1.0f and 1.0f
        //void setNextPosition(pwmType port, float orientation);

    protected:
        uint32_t enableMask;

        //the min/max pulse times that make sense for a particular PWM device
        float MIN_PULSE_TIME;
        float MAX_PULSE_TIME;
};

#endif /* SERVOTASK_HPP_ */

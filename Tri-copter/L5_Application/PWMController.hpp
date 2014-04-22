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
            pwm1=0, ///< P2.0
            pwm2=1, ///< P2.1
            pwm3=2, ///< P2.2
            pwm4=3, ///< P2.0
            pwm5=4, ///< P2.1
            pwm6=5  ///< P2.2
        } pwmType;

        PWMController();

        //Begin PWM output on this port
        bool enablePort(pwmType port);

        //Set PWM percent
        virtual void setPercent(pwmType port, float percent) = 0;

        //Sets the next orientation of the servo
        //value between -1.0f and 1.0f
        void setNextPosition(pwmType port, float orientation);

    protected:
        uint32_t enableMask;
};

#endif /* SERVOTASK_HPP_ */

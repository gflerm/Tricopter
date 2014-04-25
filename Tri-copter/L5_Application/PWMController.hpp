/*
 * ServoTask.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: sloretz
 */

#ifndef PWM_CTRL_HPP_
#define PWM_CTRL_HPP_
#include <stdint.h>

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

        //Sets the next orientation of the servo
        //value between -1.0f and 1.0f
        void setNextPosition(pwmType port, float orientation);

        //Sets the pulse width percent (0-100) between MIN_PULSE and MAX_PULSE
        void setPercent(pwmType port, float percent);

    private:
        uint32_t enableMask;

        const static float MIN_PULSE = .001; //seconds
        const static float MAX_PULSE = .002; //seconds
};

#endif /* SERVOTASK_HPP_ */

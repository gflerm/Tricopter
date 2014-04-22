/*
 * MotorController.hpp
 *
 *  Created on: Apr 22, 2014
 *      Author: Michael
 */

#ifndef MOTORCONTROLLER_HPP_
#define MOTORCONTROLLER_HPP_

#include "PWMController.hpp"

class MotorController : public PWMController
{
public:
        MotorController() : PWMController()
        {
            MIN_PULSE_TIME = 1;
            MAX_PULSE_TIME = 2;
        };
        void setPercent(pwmType port, float percent);
private:
};

#endif /* MOTORCONTROLLER_HPP_ */

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
        MotorController() : PWMController() { };
        void setPercent(pwmType port, float percent);
private:
};

#endif /* MOTORCONTROLLER_HPP_ */

/*
 * ServoController.hpp
 *
 *  Created on: Apr 22, 2014
 *      Author: Michael
 */

#ifndef SERVOCONTROLLER_HPP_
#define SERVOCONTROLLER_HPP_

#include "PWMController.hpp"

class ServoController : public PWMController
{
public:
      ServoController() : PWMController() {  };
      void setPercent(pwmType port, float percent);
private:
};


#endif /* SERVOCONTROLLER_HPP_ */

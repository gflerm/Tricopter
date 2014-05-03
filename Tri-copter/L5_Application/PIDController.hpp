/*
 * PIDController.hpp
 *
 *  Created on: May 2, 2014
 *      Author: sloretz
 */

#ifndef PIDCONTROLLER_HPP_
#define PIDCONTROLLER_HPP_

class PIDController {
public:
   PIDController(float kp, float ki, float kd);
   ~PIDController(){};

   float calculate_output(float input, float target, float dt);

   float get_kp();
   float get_ki();
   float get_kd();

protected:
   float kp, ki, kd;
   float integrator;
   float lastInput;
};

#endif /* PIDCONTROLLER_HPP_ */

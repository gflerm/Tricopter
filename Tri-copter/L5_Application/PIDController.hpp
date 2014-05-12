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
   PIDController(float kp, float ki, float kd, float ks);
   ~PIDController(){};

   float calculate_output(float input, float target, float dt);

   void updateConstants(float _kp, float _ki, float _kd, float _ks);

   float get_kp();
   float get_ki();
   float get_kd();
   float get_ks();

protected:
   float kp, ki, kd, ks;
   float integrator;
   float lastError;
};

#endif /* PIDCONTROLLER_HPP_ */

/*
 * BM_Controller.h
 *
 *  Created on: Mar 27, 2014
 *      Author: Carlos
 */

#ifndef BM_CONTROLLER_H_
#define BM_CONTROLLER_H_
#include "lpc_pwm.hpp"

#define X_ORIENTATION ((orientation_data_t*) data)->orientation.x.word
#define Y_OREINTATION ((orientation_data_t*) data)->orientation.y.word
#define Z_OREINTATION ((orientation_data_t*) data)->orientation.z.word


void set_base_oreintaiton(void* data){
    uint16_t x,y,z;


}



void motor_control(void* data){


}




#endif /* BM_CONTROLLER_H_ */

/*
 * BM_Controller.h
 *
 *  Created on: Mar 27, 2014
 *      Author: Carlos
 */

#ifndef BM_CONTROLLER_H_
#define BM_CONTROLLER_H_
#include "lpc_pwm.hpp"

#define LEFT_MOTOR 		((class motors*) p)->left_motor
#define RIGHT_MOTOR 	((class motors*) p)->right_motor
#define REAR_MOTOR		((class motors*) p)->rear_servo

/*class motors{
public:
PWM *left_motor= new PWM(PWM::pwm2, 16000);
PWM *right_motor= new PWM(PWM::pwm3, 16000);
PWM *rear_motor= new PWM(PWM::pwm4, 16000);
PWM *rear_servo= new PWM(PWM::pwm5, 8000);
};
*/
PWM servo(PWM::pwm2, 5);

void calibrate_esc(void *p){
	int thing=0;
    while(1){
	    printf("enter duty\n");
	    scanf("%i", &thing);
	    printf("%i", thing);
	    servo.set(thing);

	}
}








#endif /* BM_CONTROLLER_H_ */

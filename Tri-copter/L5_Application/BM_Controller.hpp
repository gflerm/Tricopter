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

class motors{
public:
PWM *left_motor= new PWM(PWM::pwm2, 16000);
PWM *right_motor= new PWM(PWM::pwm3, 16000);
PWM *rear_motor= new PWM(PWM::pwm4, 16000);
PWM *rear_servo= new PWM(PWM::pwm5, 8000);
};

void calibrate_esc(void *p){
	int input;
	//vTaskSuspend(NULL);
	while(1){
	printf("\nMake sure motors are disconnected from power.Press ENTER to begin calibration.\n");
	scanf("%i", &input);

	LEFT_MOTOR->set(100);
	RIGHT_MOTOR->set(100);
	REAR_MOTOR->set(100);

	printf("\nNow, provide motors with power, once final beep heard press ENTER\n");
	scanf("%i", &input);

	LEFT_MOTOR->set(0);
	RIGHT_MOTOR->set(0);
	REAR_MOTOR->set(0);

	printf("\nCalibration should now be complete!!!\n");
	//vTaskSuspend(NULL);
	}
}








#endif /* BM_CONTROLLER_H_ */

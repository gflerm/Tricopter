/*
 * 10dof.h
 *
 *  Created on: Mar 29, 2014
 *      Author: Carlos
 */

#ifndef DOF_H_
#define DOF_H_

#define ACCEL_ADDRESS 0xA7
#define GYRO_ADDRESS 0xD3
#define X_AXIS_ACCEL  ((struct balance*) p)->X_axis.accel.word
//#define X_AXIS_GYRO
//#define X_AXIS_MAGNO
#define Y_AXIS_ACCEL  ((struct balance*) p)->Y_axis.accel.word
#define Z_AXIS_ACCEL  ((struct balance*) p)->Z_axis.accel.word
//#define

union axis_info{
	char byte[2];
	uint16_t word;
};

struct Axis{
	axis_info accel;
	axis_info gyro;
	axis_info magno;
};

struct balance{
	Axis X_axis;
	Axis Y_axis;
	Axis Z_axis;
};


static I2C2 &axis = I2C2::getInstance();

void get_x(balance *x){
	axis.readRegisters(ACCEL_ADDRESS, 0x32, x->X_axis.accel.byte,2);
};

void get_y(balance *y){
	axis.readRegisters(ACCEL_ADDRESS, 0x34, y->Y_axis.accel.byte,2);
}

void get_z(balance *z){
	axis.readRegisters(ACCEL_ADDRESS, 0x36, z->Z_axis.accel.byte,2);
}


void display_values(void *p){
	vTaskSuspend(NULL); // Wait to be called from terminal
	uint8_t count=0;
	vTaskSuspend(NULL);
	while(1){
		get_x((struct balance*) p);
		get_y((struct balance*) p);
		get_z((struct balance*) p);
		printf("\nX: %d \n Y: %d \n Z: %d\n", X_AXIS_ACCEL, Y_AXIS_ACCEL, Z_AXIS_ACCEL);
		//count == 10 ? count=0 ,vTaskSuspend(NULL) : count++;
		vTaskSuspend(NULL);
	}

}


void user_calibration(void *p){
	int input=0;
	axis.writeReg(0xA6, 0x2D, 0x08); // Accelerometer power
	axis.writeReg(0xA6, 0x31, 0x08);
	char mean [3];
	bool cont=false;
	vTaskSuspend(NULL);// Suspend until called from terminal
	while(1){
	printf("\n*****Tri-Copter Calibration********\n\n""");
	printf("\n*****Choose Sensor:\n 1.Accel \n 2. Gyro \n 3. Magno\n\n\n\n");
	scanf("%i", &input);
	switch(input){
		case 1:
			printf("\nPlace Sensor flat and avoid movement.\n Press ENTER when ready\n");
			scanf("%i", &input);
			do{
			for(int i=0; i<10; i++)
			{
				get_x(((struct balance*) p));
				mean[0]+=X_AXIS_ACCEL;
			}
				mean[0]=mean[0]/10;

			for(int i=0; i<10; i++)
			{
				get_y(((struct balance*)p));
				mean[1]+=Y_AXIS_ACCEL;
			}
				mean[1]=mean[1]/10;

			for(int i=0; i<10; i++)
			{
				get_z((struct balance*)p);
				mean[2]+=Z_AXIS_ACCEL;
			}
				mean[2]=mean[2]/10;

			printf("\nDONE!\n X Axis: %i \n Y Axis: %i \n Z Axis: %i \n Happy with results?\n", mean[0], mean[1], mean[2]);
				scanf("%i", &input);
					input == 1 ? cont=true : cont=false;

			} while(cont==false);
			//negate values for offset
			mean[0]=-mean[0];
			mean[1]=-mean[1];
			mean[2]=-mean[2];
			axis.writeRegisters(0xA6, 0x1E, mean, 3); //Write to offset register
			break;

		case 2:

			break;

		case 3:

			break;

		default:
			break;



	}
	vTaskSuspend(NULL);
	}
}


//0x53 accelormater adress
//0x32 x axis
//0x34 y axis
//0x36 z axis

//0x34 gyro adress
//0x28 x axis
//0x2a y axis
//0x2c z axis







#endif /* 10DOF_H_ */

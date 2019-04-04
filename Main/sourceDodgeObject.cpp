//
// Created by kevin on 29/03/19.
//

#include "linefollower.h"

using namespace std;

int current_position_us=0;
// TODO: --> Complete remake
// Robot rijdt linksom het object
// TODO: --> hoofd draaien & rijden
// TODO: --> Als die over een lijn rijdt en een bocht maakt, hoofd kijkt mee
// TODO: --> Bepalen hoe te sturen bij zien voorwerp
// TODO: --> Lijn terugvinden na object & niet tegen het object aan rijden.


void turnUS (float values_wheels){
	//values_wheels een waarde tussen 0 & 2
	int max_degrees_turned = 95;
	int min_degrees_turned = -95;
	int target_degrees_us = 0;
	int degrees_to_turn = 0;
	uint8_t motor_middle = PORT_B; // Setting the motor to communicate

	if(values_wheels == 1){
		target_degrees_us = 0;
	} else if(values_wheels < 1){
		target_degrees_us = min_degrees_turned * (1-values_wheels);
	} else{
		target_degrees_us = max_degrees_turned/values_wheels;
	}

	degrees_to_turn = target_degrees_us - current_position_us;
	BP.set_motor_limits(motor_middle, 100, 90); // Limit the motor so the gears can keep up
	BP.set_motor_position_relative(motor_middle, target_degrees_us);
}





void drive_straight(int32_t to_drive, BrickPi3 & BP){
	uint8_t motor_2 = PORT_D;
	uint8_t motor_1 = PORT_A;
	//BP.set_motor_limits(motor_1,60,180);
	//BP.set_motor_limits(motor_2,60,180);
	BP.set_motor_position_relative(motor_1,to_drive);
	BP.set_motor_position_relative(motor_2,to_drive);
	usleep(1000*(2.2*to_drive));
}

void driveCurved(int32_t to_drive, BrickPi3 & BP) {
	uint8_t motor_2 = PORT_D;
	uint8_t motor_1 = PORT_A;
	BP.set_motor_position_relative(motor_1, to_drive);
	BP.set_motor_position_relative(motor_2, (to_drive * 1.2));
	usleep(1000*(2.2*to_drive));
}

void driveAround(BrickPi3 & BP) {
    driveOnSpot('R', BP);
    drive_straight(400,BP);
    driveOnSpot('L', BP);
    drive_straight(1000,BP);
    driveOnSpot('L', BP);
    drive_straight(385,BP);
    driveOnSpot('R', BP);
}

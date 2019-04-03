//
// Created by kevin on 29/03/19.
//

#include "linefollower.h"

using namespace std;

// TODO: --> Complete remake

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

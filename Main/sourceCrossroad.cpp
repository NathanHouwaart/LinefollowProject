/*
---------------------------------------------------------------------------------------------
-----Created by TICT-1C groep 1                                                         -----
-----Lego mindstorms linefollower robot project                                         -----
-----Contributors:                                                                      -----
-----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart  -----
 ---------------------------------------------------------------------------------------------
*/

#include "linefollower.h"

using namespace std;

void crossLine(BrickPi3 & BP, int32_t forward_degrees){
//	cout << "Ik ben crossline" << endl;
	uint8_t motor_right = PORT_D;
	uint8_t motor_left = PORT_A;
	BP.set_motor_position_relative(motor_left, forward_degrees);
	BP.set_motor_position_relative(motor_right, forward_degrees);
	usleep(1000*400);
}

void driveLeft(BrickPi3 & BP) {
	crossLine(BP,160);
	driveOnSpot('L',BP);
}

void driveRight(BrickPi3 & BP) {
	crossLine(BP,90);
	driveOnSpot('R',BP);
}

void driveForward(BrickPi3 & BP) {
	crossLine(BP,90);
}

void crossroad(BrickPi3 & BP) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    char choice;
    cin >> choice;
    switch (choice) {
        case 'L':
            driveLeft(BP);
            break;
        case 'R':
            driveRight(BP);
            break;
        case 'F':
            driveForward(BP);
            break;
        default:
            cout << "Wrong input. Please try again";
            crossroad(BP);
    }
}

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
	uint8_t motor_right = PORT_D;
	uint8_t motor_left = PORT_A;
//	BP.set_motor_limits(PORT_D, 100, 90);
//	BP.set_motor_limits(PORT_A, 100, 90);
	BP.set_motor_position_relative(motor_left, forward_degrees);
	BP.set_motor_position_relative(motor_right, forward_degrees);
	usleep(1000*400);
}

void driveLeft(BrickPi3 & BP, int & playing) {         //skip over line and turn left
	crossLine(BP,240);
    playSound('T', playing);
	usleep(1000*400);
	driveOnSpot('L',BP);
	crossLine(BP, 40);
}

void driveRight(BrickPi3 & BP, int & playing) {        //skip over line and turn right
	crossLine(BP,90);
	playSound('T', playing);
	driveOnSpot('R',BP);
}

void driveForward(BrickPi3 & BP, int & playing) {      //skip over line
	crossLine(BP,90);
    playSound('T', playing);
}

void crossroad(BrickPi3 & BP, int & playing) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    setLcd('Which way', 'R, L, F');
    char choice;
    cin >> choice;
    switch (choice) {
        case 'L':
            driveLeft(BP, playing);
            setLcd('going left' , ' ');
            break;
        case 'R':
            driveRight(BP, playing);
            setLcd('going right' , ' ');
            break;
        case 'F':
            driveForward(BP, playing);
            setLcd('going forward' , ' ');
            break;
        default:
            cout << "Wrong input. Please try again";
            setLcd('Wrong input' , 'try again');
            usleep(500);
            crossroad(BP, playing);
    }
}

void crossroad(BrickPi3 & BP, const char & direction_instruction, int & playing) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    switch (direction_instruction) {
        case 'L':
            driveLeft(BP, playing);
            setLcd('Going left' , ' ');
            break;
        case 'R':
            driveRight(BP, playing);
            setLcd('Going right' , ' ');
            break;
        case 'F':
            driveForward(BP, playing);
            setLcd('Going forward' , ' ');
            break;
        default:
            cout << "Wrong input. Please try again";
            setLcd('Wrong input' , 'try again');
            usleep(500);
            crossroad(BP, playing);
    }
}


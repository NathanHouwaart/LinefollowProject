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
        playSound('L', playing);
	usleep(1000*400);       //delay so sound has enough time to play
	driveOnSpot('L',BP);
	crossLine(BP, 40);
}

void driveRight(BrickPi3 & BP, int & playing) {        //skip over line and turn right
	crossLine(BP,90);
	playSound('R', playing);
	usleep(1000*1000);       //delay so sound has enough time to play
	driveOnSpot('R',BP);
}

void driveForward(BrickPi3 & BP, int & playing) {      //skip over line
    playSound('S', playing);
    usleep(1000*1000);      //delay so sound has enough time to play
    crossLine(BP,90);
}

void crossroad(BrickPi3 & BP, int & playing, int & fd) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    clearLcd(fd);   // clear the lcd
    cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
    typeString("Which way", fd);   // print the text on the screen
    cursorLocation(LINE2, fd);
    typeString("R, L, F", fd);
    char choice;
    cin >> choice;
    switch (choice) {
        case 'L':
            driveLeft(BP, playing);
            break;
        case 'R':
            driveRight(BP, playing);
            break;
        case 'F':
            driveForward(BP, playing);
            break;
        default:
            cout << "Wrong input. Please try again";
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeString("wrong input", fd);   // print the text on the screen
            cursorLocation(LINE2, fd);      // set the cursorlocation to line 2
            typeString("try again", fd);            // print the text to the screen
            crossroad(BP, playing, fd);
    }
}

void crossroadGrid(BrickPi3 & BP, const char & direction_instruction, int & playing, int & fd) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    switch (direction_instruction) {
        case 'L':
            driveLeft(BP, playing);
            break;
        case 'R':
            driveRight(BP, playing);
            break;
        case 'F':
            driveForward(BP, playing);
            break;
        default:
            cout << "Wrong input. Please try again";
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeString("wrong input", fd);   // print the text on the screen
            cursorLocation(LINE2, fd);      // set the cursorlocation to line 2
            typeString("try again", fd);            // print the text to the screen
            usleep(500);
            crossroad(BP, playing, fd);
    }
}

void crossroad(BrickPi3 & BP, const char & direction_instruction) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    switch (direction_instruction) {
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


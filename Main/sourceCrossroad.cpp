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

void crossLine(BrickPi3 & BP, int32_t forward_degrees) {
    /* This function makes the robot drive the robot a given distance. */
	uint8_t motor_right = PORT_D;                                   // Setting the motor port to communicate with motor
	uint8_t motor_left = PORT_A;                                    // Setting the motor port to communicate with motor
	BP.set_motor_position_relative(motor_left, forward_degrees);    // Turns the motor the given distance
	BP.set_motor_position_relative(motor_right, forward_degrees);   // Turns the motor the given distance
	usleep(1000*400);                                               // Sleep to let the robot drive forward
}

void driveLeft(BrickPi3 & BP, int & playing) {
	/* This function plays the apprioate sound.
	 * After that the robot drives forward,turns 90 degrees to left on axis and drives a small distance forward.
	 */
    playSound('L', playing);                                        // Plays the appropriate sound
    crossLine(BP,240);
	usleep(1000*400);                                               // Delay so robot can drive forward
	driveOnSpot('L',BP);                                            // Robot turns on spot
	crossLine(BP, 40);
}

void driveRight(BrickPi3 & BP, int & playing) {        //skip over line and turn right
    /* This function plays the apprioate sound.
	 * After that the robot drives forward and turns 90 degrees to right.
	 */
    playSound('R', playing);                                        // Plays the appropriate sound
	crossLine(BP,90);
	usleep(1000*400);                                               // Delay so sound robot can drive forward
	driveOnSpot('R',BP);                                            // Robot turns on spot
}

void driveForward(BrickPi3 & BP, int & playing) {
    /* Play a sound and cross over the line. */
    playSound('S', playing);                                        // Plays the appropriate sound
    usleep(1000*1000);                                              // Delay so sound has enough time to play
    crossLine(BP,90);
}

void crossroad(BrickPi3 & BP, int & playing, int & fd) {
    /*
     *
     */

    drive(DIRECTION_STOP, 0, 360, BP);                              // Stop the robot
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    clearLcd(fd);                                                   // clear the LCD
    cursorLocation(LINE1, fd);                                      // Set the cursorlocation to line 1
    typeString("Which way:", fd);                                   // Print the text on the screen
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
            clearLcd(fd);                               // clear the lcd
            cursorLocation(LINE1, fd);                  // set the cursorlocation to line 1
            typeString("wrong input", fd);              // print the text on the screen
            cursorLocation(LINE2, fd);                  // set the cursorlocation to line 2
            typeString("try again", fd);                // print the text to the screen
            usleep(500);
            crossroad(BP, playing, fd);
    }
}


/*
or: too few arguments to function ‘void d---------------------------------------------------------------------------------------------
-----Created by TICT-1C groep 1                                                         -----
-----Lego mindstorms linefollower robot project                                         -----
-----Contributors:                                                                      -----
-----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart  -----
 ---------------------------------------------------------------------------------------------
*/

#include "linefollower.h"

using namespace std;

void crossLine(int32_t forward_degrees, BrickPi3 & BP) {
    /* This function makes the robot drive the robot a given distance. */
	uint8_t motor_right = PORT_D;                                   // Setting the motor port to communicate with motor
	uint8_t motor_left = PORT_A;                                    // Setting the motor port to communicate with motor
	BP.set_motor_position_relative(motor_left, forward_degrees);    // Turns the motor the given distance
	BP.set_motor_position_relative(motor_right, forward_degrees);   // Turns the motor the given distance
	usleep(1000*400);                                               // Sleep to let the robot drive forward
}

void driveLeft(int & playing, BrickPi3 & BP) {
	/* This function plays the apprioate sound.
	 * After that the robot drives forward,turns 90 degrees to left on axis and drives a small distance forward.
	 */
    playSound('L', playing);                                        // Plays the appropriate sound
    crossLine(240,BP);
	usleep(1000*400);                                               // Delay so robot can drive forward
	driveOnSpot('L',BP);                                            // Robot turns on spot
	crossLine(40,BP);
}

void driveRight(int & playing, BrickPi3 & BP) {
    /* This function plays the apprioate sound.
	 * After that the robot drives forward and turns 90 degrees to right.
	 */
    playSound('R', playing);                                        // Plays the appropriate sound
	crossLine(90,BP);
	usleep(1000*400);                                               // Delay so sound robot can drive forward
	driveOnSpot('R',BP);                                            // Robot turns on spot
}

void driveForward(int & playing, BrickPi3 & BP) {
    /* Play a sound and cross over the line. */
    playSound('S', playing);                                        // Plays the appropriate sound
    usleep(1000*1000);                                              // Delay so sound has enough time to play
    crossLine(90,BP);
}

void crossroad(int & playing, int & fd_lcd, BluetoothSocket* clientsock, BrickPi3 & BP) {
    /* The function handles the situation when the robot detects a crossroad in line follow-mode.
     * It Prints a message on the LCD screen and waits on a message from the bluetooth connection.
     * It takes the message and decides based on that what the direction the robot needs to go.
     */
    drive(DIRECTION_STOP, 0, 360, BP);                              // Stop the car
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    clearLcd(fd_lcd);                                                   // Clear the LCD
    cursorLocation(LINE1, fd_lcd);                                      // Set the cursorlocation to line 1
    typeString("Which way:", fd_lcd);                                   // Print the text on the screen
    cursorLocation(LINE2, fd_lcd);                                      // Set the cursorlocation to line 2
    typeString("R, L, F", fd_lcd);                                      // Print the text on the screen

    MessageBox& mb = clientsock->getMessageBox();                   // Initialise the message box
    string input;                                                   // Save the input from the message box
    char driving_direction_control;                                 // Variable that controls driving direction of robot

    /*-----The while loop is active as long there is a bluetooth connection that is active------*/
    while(mb.isRunning()) {
        input = mb.readMessage();                                   // Puts message box in string
        if(input == "LEFT") {
            // Input is left so robot needs to turn to the left on crossroad
            cout << input << endl;
            driving_direction_control = 'L';                        // Puts correct char in the driving direction control
            break;                                                  // Make sure we leave loop because input correct
        } else if (input == "RIGHT") {
            // Input is right so robot needs to turn to the right on crossroad
            cout << input << endl;
            driving_direction_control = 'R';                        // Puts correct char in the driving direction control
            break;                                                  // Make sure we leave loop because input correct
        } else if (input == "UP") {
            // Input is up so robot needs to drive straight on the crossroad
            cout << input << endl;
            driving_direction_control = 'F';                        // Puts correct char in the driving direction control
            break;                                                  // Make sure we leave loop because input correct
        }
        cout << ".";                                                // Places a dot for every given input (counter)
        cout.flush();                                               // Forces the cout buffer to the output
	    usleep(200*1000);                                           // Wait 200 ms
    }

    switch (driving_direction_control) {
        case 'L':
            // The robot needs to drive to the left, calls correct function
            driveLeft(playing, BP);
            break;
        case 'R':
            // The robot needs to drive to the right, calls correct function
            driveRight(playing, BP);
            break;
        case 'F':
            // The robot needs to drive straight on, calls correct function
            driveForward(playing, BP);
            break;
        default:
            // Given input is wrong so print error and call function again
            cout << "Wrong input. Please try again";
            clearLcd(fd_lcd);                                           // Clear the lcd
            cursorLocation(LINE1, fd_lcd);                              // Set the cursorlocation to line 1
            typeString("wrong input", fd_lcd);                          // Print the text on the screen
            cursorLocation(LINE2, fd_lcd);                              // Set the cursorlocation to line 2
            typeString("Try again", fd_lcd);                            // Print the text to the screen
            usleep(1000*500);                                       // Wait a bit before calling the function again
            crossroad(playing, fd_lcd, clientsock, BP);
    }
}

void crossroadGrid(const char & direction_instruction, int & playing, int & fd_lcd, BrickPi3 & BP) {
    /* The function handles the situation when the robot detects a crossroad in grid navigate-mode.
     * The function expects a direction_control, the variable decides which way the robot will go.
     */
    drive(DIRECTION_STOP, 0, 360, BP);                              // Stop the robot
    switch (direction_instruction) {
        case 'L':
            // Input is left so robot needs to turn to the left on crossroad
            driveLeft(playing, BP);
            break;
        case 'R':
            // The robot needs to drive to the right, calls correct function
            driveRight(playing, BP);
            break;
        case 'F':
            // The robot needs to drive straight on, calls correct function
            driveForward(playing, BP);
            break;
        default:
            // Given input is wrong so print error and call function again
            cout << "Wrong input. Please try again";
            clearLcd(fd_lcd);                                           // Clear the lcd
            cursorLocation(LINE1, fd_lcd);                              // Set the cursorlocation to line 1
            typeString("wrong input", fd_lcd);                          // Print the text on the screen
            cursorLocation(LINE2, fd_lcd);                              // Set the cursorlocation to line 2
            typeString("try again", fd_lcd);                            // Print the text to the screen
            usleep(1000*500);                                       // Wait before calling function again
            crossroadGrid(direction_instruction, playing, fd_lcd, BP);
    }
}

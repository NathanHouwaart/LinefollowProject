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

void exit_signal_handler(int signo);
BrickPi3 BP;

int main() {
    /*-----Declaration of the variables used in the main------*/
    char mode_select;                                       // Variable to save the answer of the user
    bool correct_answer = false;                            // Enables to keep asking for user input even if answer incorrect

    /*-----Setup exit handler and detect BrickPi-----*/
    signal(SIGINT, exit_signal_handler);                    // Register the exit function for Ctrl+C & cleanup
    BP.detect();                                            // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    /*-----Set sensor types-----*/
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);  // Set port 1 to be read as a color sensor
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_COLOR_RED);  // Set port 3 to be read as a color sensor
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // Set port 4 to be read as a ultrasonic sensor

    /*-----Set the lcd pins and the start the lcd------*/
    if (wiringPiSetup () == -1) {
        // Fallback if something wrong with setting up the GPIO pins with Wiring Pi
        cout << "wiringpi-setup did not work" << endl << "return 1;" << endl;
        return 1;
    }

    /*-----Setting up the LCD screen-----*/
    int address_lcd = wiringPiI2CSetup(I2C_ADDR);           // Gives back the address of the LCD
    lcdStart(address_lcd);                                  // setup LCD
    clearLcd(address_lcd);                                  // clear the lcd
    cursorLocation(LINE1, address_lcd);                     // set the cursorlocation to line 1
    typeString("Setting up", address_lcd);                  // print the text on the screen

    /*-----Set data structs-----*/
    sensor_color_t Color1;                                  // Initialise struct for data storage color sensor 1
    sensor_color_t Color2;                                  // Initialise struct for data storage color sensor 2
    sensor_ultrasonic_t UltraSonic1;                        // Initialise struct for data storage ultrasonic sensor 1
    CalculatingErrorData DataLineMeasurments;               // Initialise struct for data storage average, min and max reading of Color1

    /*-----Checks whether battery has enough power, if not exit program-----*/
    if(!checkVoltage(BP)) return 0;

    clearLcd(address_lcd);                                  // Clear the LCD
    cursorLocation(LINE1, address_lcd);                     // Set the cursorlocation to line 1
    typeString("Calibrating", address_lcd);                 // Print the text on the screen

    /*-----Calibrate min and max reflection values and determine lightvalue the robot wants to follow-----*/
    calibration(Color1, DataLineMeasurments, BP);           // Gives back lowest and highest value
    defineDifferenceToAverage(DataLineMeasurments);         // Calculate the different values, puts values in struct
    usleep(1000*1000);                                      // Waiting for sensors to see normally
    clearLcd(address_lcd);                                  // Clear the LCD

    /*-----The while loop is active as long the given answer is incorrect (false)-----*/
    while(!correct_answer){
        /*-----Print question on the LCD-----*/
        clearLcd(address_lcd);                              // Clear the LCD
        cursorLocation(LINE1, address_lcd);                 // Set the cursorlocation to line 1
        typeString("Select mode:", address_lcd);            // Print the text on the screen
        cursorLocation(LINE2, address_lcd);                 // Set the cursorlocation to line 2
        typeString("L G F", address_lcd);                   // Print the text to the screen
        /*-----Print question on terminal-----*/
        cout << "Select mode: Line follow (L) / grid follow (G) / Free ride (F)" << endl;
        cin >> mode_select;                                 // Save output from user

        switch (mode_select) {
            case 'L':
                // The robot enters the line follow-mode
                correct_answer = true;                      // Makes sure program leaves loop
                cout << "Entering the line follow-mode." << endl;
                lineFollowLoop(Color1, Color2, UltraSonic1, DataLineMeasurments, address_lcd, BP);
                break;
            case 'G':
                // The robot enters the grid navigate-mode
                correct_answer = true;                      // Makes sure program leaves loop
                cout << "Entering the grid navigate-mode." << endl;
                gridFollowLoop(Color1, Color2, UltraSonic1, DataLineMeasurments, address_lcd, BP);
                break;
            case 'F':
                // The robot enters the free ride-mode
                correct_answer = true;                      // Makes sure program leaves loop
                cout << "Entering the free ride-mode." << endl;
                freeRideLoop(address_lcd, BP);
                break;
            default:
                // Input of user is incorrect, print error message on terminal and LCD
                /*-----Print error on LCD-----*/
                clearLcd(address_lcd);                          // Clear the LCD
                cursorLocation(LINE1, address_lcd);             // Set the cursorlocation to line 1
                typeString("ERROR:", address_lcd);              // Print the text on the screen
                cursorLocation(LINE2, address_lcd);             // Set the cursorlocation to line 2
                typeString("Wrong input user", address_lcd);    // Print the text to the screen
                /*-----Print error on terminal-----*/
                cout << "ERROR, wrong input" << endl;
                usleep(1000*2000);                              // Waits a bit to show message on LCD
                break;
        }
   }
}

void exit_signal_handler(int signo) {
    /* This code will run when exiting the program with ctr + c. Used to reset de BrickPi outputs.
     * The declaration of this functions must be in the main.cpp file
     */
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

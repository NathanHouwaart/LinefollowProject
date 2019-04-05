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
    /*-----Setup exit handler and detect BrickPi-----*/
    signal(SIGINT, exit_signal_handler);                        // Register the exit function for Ctrl+C & cleanup
    BP.detect();                                                // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    /*-----Set sensor types-----*/
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);      // Set port 1 to be read as a color sensor
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_COLOR_RED);      // Set port 3 to be read as a color sensor
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);     // Set port 4 to be read as a ultrasonic sensor

    /*-----Set the lcd pins and the start the lcd------*/
    if (wiringPiSetup () == -1) {
        cout << "wiringpisetup did not work" << endl << "return 1;" << endl;
        return 1;
    }
    fd = wiringPiI2CSetup(I2C_ADDR);
    lcd_init(); // setup LCD

    /*-----Set data structs-----*/
    sensor_color_t Color1;                                      // Initialise struct for data storage color sensor 1
    sensor_color_t Color2;                                      // Initialise struct for data storage color sensor 2
    sensor_ultrasonic_t UltraSonic1;

     if(!checkVoltage(BP)) return 0;                    // Checks whether battery has enough power

    /*-----Calibrate min and max reflection values and determine lightvalue the robot wants to follow-----*/
    CalculatingErrorData struct_line_values;
    calibration(Color1, struct_line_values, BP);
    defineDifferenceToAverage(struct_line_values);
    setLcd('calibration', 'Done');    // wirting to the lcd Done
    sleep(1); //Waiting for sensors to see normally
    ClrLcd();           // Clear the lcd
	char modeselect;
	/*-----Follow the line untill the ultrasonic sensor measures something withing X cm-----*/
    cout << "Select mode: (Line follow (L) / grid follow (G) / Free ride (F))" << endl;
    cin >> modeselect;
    switch (modeselect){
        case 'L':
            setLcd('Linefollow', 'mode');
            lineFollowLoop(Color1, Color2, UltraSonic1, struct_line_values, BP);
            break;
        case 'G':
            setLcd('Gridfollow', 'mode');
            gridFollowLoop(Color1, Color2, UltraSonic1, struct_line_values, BP);
            break;
        case 'F':
            setLcd('Freeride', 'mode');
            freeRideLoop(BP);
            break;
        default:
            setLcd('ERROR goodbye', 'Have a nice day');
            cout << "ERROR, wrong input" << endl;
            return -1;
   }
}

void exit_signal_handler(int signo) {
    /*-----This code will run when exiting the program with ctr + c. Used to reset de BrickPi outputs-----*/
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        setLcd('You are' , 'killimg me');
        exit(-2);
    }
}

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

    /*-----Set data structs-----*/
    sensor_color_t Color1;                                      // Initialise struct for data storage color sensor 1
    sensor_color_t Color2;                                      // Initialise struct for data storage color sensor 2
    sensor_ultrasonic_t UltraSonic1;

     if(!checkVoltage(BP)) return 0;                          // Checks whether battery has enough power

    /*-----Calibrate min and max reflection values and determine lightvalue the robot wants to follow-----*/
    vector<int> min_max_reflection_value = calibration(Color1, BP);
    vector<int> default_values = defineDifferenceToAverage((min_max_reflection_value[0]), (min_max_reflection_value[1]));
    sleep(1); //Waiting for sensors to see normally
	char modeselect;
	/*-----Follow the line untill the ultrasonic sensor measures something withing X cm-----*/
    // TODO: --> Fuctie maken die een keuze aanbied aan de gebruiker. (LINE/GRID/FREE) (char terug L/G/F)
    // TODO: --> Modus selecteren (3 soorten while loops)
    cout << "Select mode: (Line follow (L) / grid follow (G) / Free ride (F))" << endl;
    getline(cin, modeselect);
    switch (modeselect){
        case 'L':
            lineFollowLoop(BP);
            break;
        case 'G':
            gridFollowLoop(BP);
            break;
        case 'F':
            freeRideLoop(BP);
            break;
        default:
            cout << "ERROR, wrong input" << endl;
            return -1;

    }
}

void exitSignalHandler(int signo) {
    /*-----This code will run when exiting the program with ctr + c. Used to reset de BrickPi outputs-----*/
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

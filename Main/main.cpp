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

    // if(!checkVoltage(BP)) return 0;                          // Checks whether battery has enough power

    /*-----Calibrate min and max reflection values and determine lightvalue the robot wants to follow-----*/
    vector<int> min_max_reflection_value = calibration(Color1, BP);
    vector<int> default_values = defineDifferenceToAverage((min_max_reflection_value[0]), (min_max_reflection_value[1]+100));
    sleep(1);

    /*-----Follow the line untill the ultrasonic sensor measures something withing X cm-----*/
    while (true) {
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
	BP.get_sensor(PORT_3, Color2);
        if(getUltraSValue(PORT_4, UltraSonic1, BP) > 10){       // If the measured US distance is bigger than 10:
            if(Color2.reflected_red < (default_values[0]-100) && Color1.reflected_red < default_values[0]){
                // Uncomment for debug
                // cout << "Kruispunt detected" << endl;
		cout << "Color 1: " << Color1.reflected_red << " color2: " << Color2.reflected_red << endl;
                crossroad(BP);
                // sleep(0.5); <<-- Doesnt work --> float 0,5 to int = 0.
            }else {                                             // If no intersection was detected, follow the line
                int stuurwaarde = defineDirection(
                        default_values[0],
                        default_values[1],
                        default_values[2],
                        Color1.reflected_red);

                stuur(stuurwaarde, BP);
            }
        }else{                                                  // If an object was detected within X cm, execute this code
            drive(DIRECTION_STOP, 0, 360, BP);                  // Stop the car
        }
    }
    return 0;
}

void exit_signal_handler(int signo) {
    /*-----This code will run when exiting the program with ctr + c. Used to reset de BrickPi outputs-----*/
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

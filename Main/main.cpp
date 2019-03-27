//
// Created by Nathan Houwaart on 2019-03-25.
//

#include "linefollower.h"

using namespace std;

void exit_signal_handler(int signo);

BrickPi3 BP;

int main() {

    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED); // set port 1 to be read as a color sensor
    sensor_color_t Color1;                             // Initialise struct for data storage

    // if(!checkVoltage(BP)) return 0; // Checks whether battery has enough power

    int gemiddelde, laagste_verschil, hoogste_verschil;
        vector<int> cali = calibartion(BP, Color1);                                     // For the calibration values
        vector<int> standaardwaardes = defineDifference(cali[1], cali[0]);               // To define the difference, cali[1] is high and cali[0] is the low value
    
    while (true) {
        int stuurwaarde = defineDirection(standaardwaardes[0], standaardwaardes[1], standaardwaardes[2],
                                            Color1.reflected_red);
        stuur(stuurwaarde, BP);
    }
    return 0;
}

void exit_signal_handler(int signo) {
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

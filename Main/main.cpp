//
// Created by Nathan Houwaart on 2019-03-25.
//

#include "UltraSonicHeader.h"

using namespace std;

void exit_signal_handler(int signo);

BrickPi3 BP;

int main() {

    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // set port 4 to be read as a ultrasonic sensor
    sensor_ultrasonic_t UltraSonic1;                             // Initialise struct for data storage

    // if(!checkVoltage(BP)) return 0; // Checks whether battery has enough power

    //looping to print the sensordata until someone uses ctrl+c
    while(true) {
        printUltraSValue(PORT_4, UltraSonic1, BP);
    }

}

void exit_signal_handler(int signo) {
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

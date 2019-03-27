//
// Created by Kevin Patist on 2019-27-03.
//
#include "linefollower.h"

using namespace std;

// getting sensordata and printing the appropriate values in one function
void printUltraSValue(uint8_t port, sensor_ultrasonic_t &data_struct, BrickPi3 & BP) {
    BP.get_sensor(port, data_struct); //Getting sensordata and storing it in a given struct
    cout << "Afstand:   " << (data_struct.cm - 1) << " cm." << endl;  //Printing the current centimeter value in the struct.
    sleep(1);
}


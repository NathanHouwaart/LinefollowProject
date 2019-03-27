//
// Created by Nathan Houwaart on 2019-03-25.
//
#include "UltraSonicHeader.h"

using namespace std;

// getting sensordata and printing the appropriate values in one function
void printUltraSValue(uint8_t port, sensor_ultrasonic_t &data_struct, BrickPi3 & BP) {
    BP.get_sensor(port, data_struct); //Getting sensordata and storing it in a given struct
    cout << "Afstand:   " << (data_struct.cm - 3) << " cm." << endl;  //Printing the current centimeter value in the struct.
    sleep(1);
}


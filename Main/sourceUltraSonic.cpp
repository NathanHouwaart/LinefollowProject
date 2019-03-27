//
// Created by Kevin Patist on 2019-27-03.
//
#include "linefollower.h"

using namespace std;

// getting sensordata and printing the appropriate values in one function
int getUltraSValue(uint8_t port, sensor_ultrasonic_t &data_struct, BrickPi3 & BP) {
    BP.get_sensor(port, data_struct); //Getting sensordata and storing it in a given struct
    cout << "Afstand:   " << (data_struct.cm - 1) << " cm." << endl;  //Printing the current centimeter value in the struct.
    return data_struct.cm-1;
}

void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold) {
    while (true) {
        int distance = getUltraSValue(PORT_4, UltraSonic1, BP);
        MotorController(180, 180, BP);

        while (distance < threshold) {
            MotorController(0, 0, BP);
            distance = getUltraSValue(PORT_4, UltraSonic1, BP);
            usleep(500 * 1000);
        }
    }
}

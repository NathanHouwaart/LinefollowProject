//
// Created by joche on 25-3-2019.
//

#ifndef US_SENSOR_H
#define US_SENSOR_H

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision>
#include <vector>

using namespace std;

// Initialise functions
void printUltraSValue(uint8_t port, sensor_val_struct_t &data_struct, BrickPi3 % BP);

#endif US_SENSOR_H

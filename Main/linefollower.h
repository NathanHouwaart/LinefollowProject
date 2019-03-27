//
// Created by joche on 25-3-2019.
//

#ifndef HERSENSTORM_C1_LINEFOLLOWER_H
#define HERSENSTORM_C1_LINEFOLLOWER_H

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision>
#include <vector>

using namespace std;

// Initialise functions
int getUltraSValue(uint8_t port, sensor_ultrasonic_t &data_struct, BrickPi3 & BP);

void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold);

bool lightsensorBlackLineDetect(sensor_light_t  & Light1, BrickPi3 & BP);

bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);

void limiter(int & right, int & left, const int & maximum_speed);

void MotorController(int left, int right, BrickPi3 & BP);

void drive(float value, float power, BrickPi3 & BP);

void stuur(int lijn_waarde, BrickPi3 & BP);

bool checkVoltage(BrickPi3 & BP);

vector<int> bepaalAfwijking(const int & hoogste, const int & laagste);

int bepaalStuurwaarde(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting);


#endif //HERSENSTORM_C1_LINEFOLLOWER_H
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
#include <time.h>

using namespace std;

// Initialise functions

bool lightsensorBlackLineDetect(sensor_light_t  & Light1, BrickPi3 & BP);
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);
void limiter(int & right, int & left, const int & maximum_speed);
void MotorController(int left, int right, BrickPi3 & BP);
void drive(float value, float power, BrickPi3 & BP);
void stuur(int lijn_waarde, BrickPi3 & BP);
bool checkVoltage(BrickPi3 & BP);
vector<int> defineDifference(const int & hoogste, const int & laagste);
int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting);
vector<int> calibartion(BrickPi3 & BP, sensor_color_t & Color1);
void driveOnSpot(char direction, BrickPi3 & BP);
void readMotorEncoders(BrickPi3 & BP);

#endif //HERSENSTORM_C1_LINEFOLLOWER_H

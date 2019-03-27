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

#define LEFT 0
#define FORWARD 1
#define RIGHT 2
#define STOP -1


// Initialise functions

bool lightsensorBlackLineDetect(sensor_light_t  & Light1, BrickPi3 & BP);
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);
void limiter(int & right, int & left, const int & maximum_speed);
void MotorController(int left, int right, BrickPi3 & BP);
void drive(float direction_control, unsigned int speed_calculator, unsigned int base_speed , BrickPi3 & BP);
void stuur(int lijn_waarde, BrickPi3 & BP);
bool checkVoltage(BrickPi3 & BP);
vector<int> defineDifference(const int & hoogste, const int & laagste);
int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting);
void crossroad(BrickPi3 & BP);
void driveForward(BrickPi3 & BP);
void driveRight(BrickPi3 & BP);
void driveLeft(BrickPi3 & BP);
vector<int> calibration(BrickPi3 & BP, sensor_color_t & Color1);

#endif //HERSENSTORM_C1_LINEFOLLOWER_H

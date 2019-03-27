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
void direction(int line_value, BrickPi3 & BP);
bool checkVoltage(BrickPi3 & BP);
vector<int> defineDifference(const int & high, const int & low);
int defineDirection(const int & averge, const int & lowest_difference, const int & highest_difference, const int & current_light)
vector<int> calibartion(BrickPi3 & BP, sensor_color_t & Color1);


#endif //HERSENSTORM_C1_LINEFOLLOWER_H

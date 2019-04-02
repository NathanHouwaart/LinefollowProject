/*
---------------------------------------------------------------------------------------------
-----Created by TICT-1C groep 1                                                         -----
-----Lego mindstorms linefollower robot project                                         -----
-----Contributors:                                                                      -----
-----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart  -----
 ---------------------------------------------------------------------------------------------
*/

#ifndef LinefollowProject_LINEFOLLOWER_H
#define LinefollowProject_LINEFOLLOWER_H

#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision>
#include <vector>
#include <time.h>

using namespace std;

#define DIRECTION_STOP -1
#define DIRECTION_LEFT 0
#define DIRECTION_FORWARD 1
#define DIRECTION_RIGHT 2
#define MAX_MOTOR_SPEED 400

// sourceCrossroad.cpp
void crossLine(BrickPi3 & BP, int32_t forward_degrees);
void driveLeft(BrickPi3 & BP);
void driveRight(BrickPi3 & BP);
void driveForward(BrickPi3 & BP);
void crossroad(BrickPi3 & BP);

// sourceDodgeObject.cpp
void drive_straight(int32_t to_drive, BrickPi3 & BP);
void driveCurved(int32_t to_drive, BrickPi3 & BP);
void driveAround(BrickPi3 & BP);

// sourceDriveRobot.cpp
void speedLimiter(int & right, int & left, const int & maximum_speed);
void MotorController(int left, int right, BrickPi3 & BP);
void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, BrickPi3 & BP);
void driveOnSpot(char turn_direction, BrickPi3 & BP);

// sourceFreeRideLoop.cpp
void freeRideLoop(BrickPi3 & BP);

// sourceGridFollowLoop.cpp
void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic1, vector<int> & min_max_reflection_value, vector<int> & default_values, BrickPi3 & BP);

// sourceLineFollowLoop.cpp
void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic1, vector<int> & min_max_reflection_value, vector<int> & default_values, BrickPi3 & BP);

// sourceMainFunctions.cpp
vector<int> calibration(sensor_color_t & Color1, BrickPi3 & BP);
bool checkVoltage(BrickPi3 & BP);
vector<int> defineDifferenceToAverage(const int & min_reflection_value , const int & max_reflection_value);

// sourcePIDController.cpp
int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting);
void stuur(int lijn_waarde, BrickPi3 & BP);

// sourceSensors.cpp
int getUltraSValue(uint8_t port, sensor_ultrasonic_t & data_struct, BrickPi3 & BP);
void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold);
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);

#endif //LinefollowProject_LINEFOLLOWER_H

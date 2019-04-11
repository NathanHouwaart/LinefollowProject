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
#include "BluetoothSocket.h"
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision>
#include <vector>
#include <time.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <cstdio>       // for clock
#include <ctime>        // for clock
// #include <stdlib.h>
#include <stdio.h>


using namespace std;

#define DIRECTION_STOP -1
#define DIRECTION_LEFT 0
#define DIRECTION_FORWARD 1
#define DIRECTION_RIGHT 2
#define MAX_MOTOR_SPEED 800
#define I2C_ADDR   0x27 // I2C device address of the lcd
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command
#define LINE1  0x80 // 1st line of lcd
#define LINE2  0xC0 // 2nd line of lcd
#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off
#define ENABLE  0b00000100 // Enable bit to enable the lcd


// This struct is used to save the data used in calculating the avarage
struct CalculatingErrorData{
    int lowest_measurment;
    int highest_measurment;
    int avarage_min_max;
    int difference_min_avarage;
    int difference_max_avarage;
};

// sourceCrossroad.cpp
void crossLine(BrickPi3 & BP, int32_t forward_degrees);
void driveLeft(BrickPi3 & BP, int & playing);
void driveRight(BrickPi3 & BP, int & playing);
void driveForward(BrickPi3 & BP , int & playing);
void crossroad(BrickPi3 & BP,  int & playing, int & fd, BluetoothSocket* clientsock);
void crossroadGrid(BrickPi3 & BP, const char & direction_instruction, int & playing, int & fd);

// sourceDodgeObject.cpp
float vectorAvarage(const vector<float> & to_calculate);
void turnUS (float values_wheels, BrickPi3 & BP);
void steeringRobot(char to_steer, BrickPi3 & BP);
void driveAroundObject(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP);


// sourceDriveRobot.cpp
void speedLimiter(int & right, int & left, const int & maximum_speed);
void MotorController(int left, int right, BrickPi3 & BP);
void MotorControllerPower(int left, int right, BrickPi3 & BP);
void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, BrickPi3 & BP);
void driveOnSpot(char turn_direction, BrickPi3 & BP);
void MotorControllerTurn(const int & turn, const int & target_power, const float & turn_modifier, BrickPi3 & BP);
vector<int> convertPowerValues(const int & speedA, const int & speedD);

// sourceFreeRideLoop.cpp
void freeRideLoop(int & fd, BrickPi3 & BP);
void selectDirection(const string & input, BrickPi3 & BP, float & steer, int & playing);

// sourceGridFollowLoop.cpp
void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP);

// sourceGridFollowFunctions.cpp
char relativeDirection(const char & current_robot_orientation, const char & absolute_direction);
void updateRobotOrientation(char & current_robot_orientation, const char & absolute_direction);
uint64_t factorial(uint64_t n);
void updateRoute(vector<char> & fastest_route, const unsigned int & index, const char & new_direction, const bool & redirect);
unsigned int possibleRoutes(const unsigned int & grid_height, const unsigned int & grid_width);
vector<char> fastestRoute(const unsigned int & grid_height, const unsigned int & grid_width);
vector<vector<char>> gridSetup(const unsigned int & grid_height, const unsigned int & grid_width);
void printGrid(const vector<vector<char>> & grid);
vector<size_t> getRobotPosition(const vector<vector<char>> & grid);
void updateRobotPosition(vector<vector<char>> & grid, const char & robot_direction, vector<char> & fastest_route, const unsigned int & index);
void updateBarrier(vector<vector<char>> & grid, const size_t & x_coordinate, const size_t & y_coordinate);
bool lookLeft(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookRight(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookForward(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP);
void whereToLook(vector<vector<char>> & grid, const char & look_direction, const char & facing_direction,
                 vector<size_t> position, sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP);

// sourceLineFollowLoop.cpp
void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP);

// sourceMainFunctions.cpp
void calibration(sensor_color_t & Color1, CalculatingErrorData & data_struct,  BrickPi3 & BP);
bool checkVoltage(BrickPi3 & BP);
void defineDifferenceToAverage(CalculatingErrorData & data_struct);

// sourcePIDController.cpp
int defineError(int avarage_lowest_highest, int dist_lowest_avarage, int dist_highest_avarage, int current_measurment);
void pController(int error_value, BrickPi3 & BP);

// sourceSensors.cpp
int getUltraSValue(uint8_t port, sensor_ultrasonic_t & data_struct, BrickPi3 & BP);
void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold);
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);

//sourceSound.cpp
void playSound(char selection, int & playing);
void megaCharge(int & playing, BrickPi3 & BP);
void stopSound(int & playing);

//sourceGridObjectDetect.cpp
bool lookLeft(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookRight(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookForward(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP);

//sourceLcd.cpp
void enableLcd(int bits, int & fd);
void lcd_byte(int bits, int mode, int & fd);
void clearLcd(int & fd);
void cursorLocation(int line, int & fd);
void typeString(const char *s, int & fd);
void lcdStart(int & fd);
void typeFloat(float myFloat, int & fd);

//sourcePController.cpp
void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct, float & target_power, float & kp, float & kd, float & ki, int & lastError, int & integral, int & offset, float turn_modifier);

//sourcePIDLoop.cpp
void stopMotor(BrickPi3 & BP);
void PIDlineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP, int & fd) {

//sourceGridFollowLoopPID.cpp
void gridFollowLoopPID(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP);

#endif //LinefollowProject_LINEFOLLOWER_H

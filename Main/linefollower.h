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

#include "BrickPi3.h"           // For BrickPi3
#include "BluetoothSocket.h"    // For the bluetooth functionality
#include <iostream>             // For cout and cin
#include <unistd.h>             // For usleep
#include <signal.h>             // For catching exit signals
#include <iomanip>              // For setw and setprecision
#include <vector>               // For the vector
#include <time.h>               // For ?
#include <wiringPiI2C.h>        // For controlling the LCD screen with I2C
#include <wiringPi.h>           // For the LCD screen
// #include <stdlib.h>          // Can we delete this one?
#include <stdio.h>              // For size_t


using namespace std;

#define DIRECTION_STOP -1       // To make reading of code clearer, used by calling the drive function
#define MAX_MOTOR_SPEED 800     // Set the max dps of the motors, is used in motor limiter
#define I2C_ADDR   0x27         // I2C device address of the LCD
#define LCD_CHR  1              // Mode - Sending data to LCD
#define LCD_CMD  0              // Mode - Sending command to LCD
#define LINE1  0x80             // 1st line of lcd
#define LINE2  0xC0             // 2nd line of lcd
#define LCD_BACKLIGHT   0x08    // Turn on the backlight of LCD
#define ENABLE  0b00000100      // Enable bit to enable the LCD


/* The struct is used to save the lowest and highest color value we got with the calibration.
 * It also saves the average of the line, (this is the target to follow). The last 2 variables save the difference
 * between the average and lowest and highest reading.
 */
struct CalculatingErrorData{                //TODO: Change name?
    int lowest_measurement;
    int highest_measurement;
    int average_min_max;
    int difference_min_average;
    int difference_max_average;
};

// The rest of the header-files contains all the declarations of functions. The declarations are sorted by source-file
// sourceCrossroad.cpp
void crossLine(BrickPi3 & BP, int32_t forward_degrees);
void driveLeft(BrickPi3 & BP, int & playing);
void driveRight(BrickPi3 & BP, int & playing);
void driveForward(BrickPi3 & BP , int & playing);
void crossroad(BrickPi3 & BP,  int & playing, int & fd, BluetoothSocket* clientsock);
void crossroadGrid(BrickPi3 & BP, const char & direction_instruction, int & playing, int & fd);

// sourceDodgeObject.cpp
float vectorAverage(const vector<float> & to_calculate);
void turnUS (float values_wheels, BrickPi3 & BP);
void steeringRobot(char to_steer, BrickPi3 & BP);
void driveAroundObject(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP);


// sourceDriveRobot.cpp
void speedLimiter(int & right, int & left, const int & maximum_speed);
void motorController(int left, int right, BrickPi3 & BP);
void motorControllerPower(int left, int right, BrickPi3 & BP);
void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, BrickPi3 & BP);
void driveOnSpot(char turn_direction, BrickPi3 & BP);
void motorControllerTurn(const int & turn, const int & target_power, const float & turn_modifier, BrickPi3 & BP);
vector<int> convertPowerValues(const int & speedA, const int & speedD);

// sourceFreeRideLoop.cpp
void freeRideLoop(int & fd, BrickPi3 & BP);
void selectDirection(const string & input, float & steer, int & playing, BrickPi3 & BP);
void soundSelection(const string & input, int & playing);

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
int defineError(int average_lowest_highest, int dist_lowest_average, int dist_highest_average, int current_measurement);
void pController(int error_value, BrickPi3 & BP);

// sourceSensors.cpp
int getUltraSValue(uint8_t port, sensor_ultrasonic_t & data_struct, BrickPi3 & BP);
void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold);
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP);

//sourceSound.cpp
void playSound(char selection, int & playing);
void megaCharge(int & playing, BrickPi3 & BP);
void stopSound(int & playing);

//sourceLcd.cpp
void enableLcd(int bits, int & fd);
void lcd_byte(int bits, int mode, int & fd);
void clearLcd(int & fd);
void cursorLocation(int line, int & fd);
void typeString(const char *s, int & fd);
void lcdStart(int & fd);
void typeFloat(float myFloat, int & fd);
void printPercentage(int & address_lcd, char current_mode, BrickPi3 & BP);

//sourcePController.cpp
void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct, float & target_power, float & kp, float & kd, float & ki, int & lastError, int & integral, int & offset, float turn_modifier);
void stopMotor(BrickPi3 & BP);

//sourcePIDLoop.cpp
void PIDlineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP, int & fd);

//sourceGridFollowLoopPID.cpp
void gridFollowLoopPID(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP);

#endif //LinefollowProject_LINEFOLLOWER_H

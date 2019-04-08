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
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define DIRECTION_STOP -1
#define DIRECTION_LEFT 0
#define DIRECTION_FORWARD 1
#define DIRECTION_RIGHT 2
#define MAX_MOTOR_SPEED 400
#define I2C_ADDR   0x27 // I2C device address of the lcd
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command
#define LINE1  0x80 // 1st line of lcd
#define LINE2  0xC0 // 2nd line of lcd
#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off
#define ENABLE  0b00000100 // Enable bit to enable the lcd
int fd;

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
void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP);

// sourceLineFollowLoop.cpp
void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP);

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

//sourceGridObjectDetect.cpp
bool lookLeft(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookRight(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP);
bool lookForward(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP);

//sourceLcd.cpp
void lcd_byte(int bits, int mode);
void enableLcd(int bits);
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line);
void typeln(const char *s);
void typeChar(char val);
void setLcd(const char message1, const char message2);
void resetLcd();
void lcdStart();

#endif //LinefollowProject_LINEFOLLOWER_H

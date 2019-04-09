#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define I2C_ADDR   0x27 // I2C device address of the lcd
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command
#define LINE1  0x80 // 1st line of lcd
#define LINE2  0xC0 // 2nd line of lcd
#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off
#define ENABLE  0b00000100 // Enable bit to enable the lcd

// This struct to save the wiringpi values for functions
struct wiringPiValues {
    int fd;
};


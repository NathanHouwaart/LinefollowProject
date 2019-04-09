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
// struct wiringPiValues {
//     int fd;
// };

int fd;





void enableLcd(int bits) {
    wiringPiValues val;
    // Toggle enable pin on LCD display
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    delayMicroseconds(500);
}


void lcd_byte(int bits, int mode)   {
    wiringPiValues val;
    //Send byte to data pins
    // bits = the data
    // mode = 1 for data, 0 for command
    int bits_high;
    int bits_low;
    // uses the two half byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

    // High bits
    wiringPiI2CReadReg8(fd, bits_high);
    enableLcd(bits_high);

    // Low bits
    wiringPiI2CReadReg8(fd, bits_low);
    enableLcd(bits_low);
}

void clearLcd() {
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

void cursorLocation(int line) {
    lcd_byte(line, LCD_CMD);
}

void typeString(const char *s) {
    while (*s) lcd_byte(*(s++), LCD_CHR);
}


void lcdStart() {
    // Initialise display
    lcd_byte(0x33, LCD_CMD); // Initialise
    lcd_byte(0x32, LCD_CMD); // Initialise
    lcd_byte(0x06, LCD_CMD); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD); // Clear display
    delayMicroseconds(500);
}

int main() {
    wiringPiValues val;
    if (wiringPiSetup() == -1) exit (1);
    fd = wiringPiI2CSetup(I2C_ADDR);
    lcdStart();
    while (true) {
        resetLcd();
        sleep(1);
        cursorLocation(LINE1);
        typeString("test");
        cursorLocation(LINE2);
        typeString("LOL");
        sleep(1);
    }
}
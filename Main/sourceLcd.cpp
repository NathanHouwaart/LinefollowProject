/*
---------------------------------------------------------------------------------------------
-----Created by TICT-1C groep 1                                                         -----
-----Lego mindstorms linefollower robot project                                         -----
-----Contributors:                                                                      -----
-----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart  -----
 ---------------------------------------------------------------------------------------------
*/

#include "linefollower.h"

using namespace std;

void enableLcd(int bits, int & fd) {
    /* Toggle pin to ENABLE on LCD display */
    usleep(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));   // Read the i2c pin
    usleep(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));  // Read the i2c pin
    usleep(500);
}

void lcd_byte(int bits, int mode, int & fd)   {
    /* Send bits to the lcd, mode 1 is for data en 0 is for command */
    int bits_high;
    int bits_low;
    // Uses the two half byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

    // High bits
    wiringPiI2CReadReg8(fd, bits_high);         // Read the i2c pin
    enableLcd(bits_high, fd);

    // Low bits
    wiringPiI2CReadReg8(fd, bits_low);          // Read the i2c pin
    enableLcd(bits_low, fd);
}

void clearLcd(int & fd) {
    /* Clear the whole LCD screen */
    lcd_byte(0x01, LCD_CMD, fd);
    lcd_byte(0x02, LCD_CMD, fd);
}

void cursorLocation(int line, int & fd) {
    /* Set the cursor location */
    lcd_byte(line, LCD_CMD, fd);
}

void typeString(const char *s, int & fd) {
    /* Push the letter 1 by 1 to the screen */
    while (*s) lcd_byte(*(s++), LCD_CHR, fd);
}


void lcdStart(int & fd) {
    /* Program to start the LCD (Configuration) */
    lcd_byte(0x33, LCD_CMD, fd);                // Tells the LCD how much space is available (Down)
    lcd_byte(0x32, LCD_CMD, fd);                // Tells the LCD how much space is available (Up)
    lcd_byte(0x06, LCD_CMD, fd);                // The direction the cursor moves to
    lcd_byte(0x0C, LCD_CMD, fd);                // 0x0F On, now Off
    lcd_byte(0x28, LCD_CMD, fd);                // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD, fd);                // Clear display
    usleep(500);
}

void typeFloat(float myFloat, int & fd) {
    /* This function gets a float and changes it to a char */
    char buffer[20];
    sprintf(buffer, "%4.2f",  myFloat);
    typeString(buffer, fd);
}

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

void enableLcd(int bits, int & fd_lcd) {
    /* Toggle pin to ENABLE on LCD display */
    usleep(500);
    wiringPiI2CReadReg8(fd_lcd, (bits | ENABLE));   // Read the i2c pin
    usleep(500);
    wiringPiI2CReadReg8(fd_lcd, (bits & ~ENABLE));  // Read the i2c pin
    usleep(500);
}

void lcd_byte(int bits, int mode, int & fd_lcd)   {
    /* Send bits to the lcd, mode 1 is for data en 0 is for command */
    int bits_high;
    int bits_low;
    // Uses the two half byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

    // High bits
    wiringPiI2CReadReg8(fd_lcd, bits_high);         // Read the i2c pin
    enableLcd(bits_high, fd_lcd);

    // Low bits
    wiringPiI2CReadReg8(fd_lcd, bits_low);          // Read the i2c pin
    enableLcd(bits_low, fd_lcd);
}

void clearLcd(int & fd_lcd) {
    /* Clear the whole LCD screen */
    lcd_byte(0x01, LCD_CMD, fd_lcd);
    lcd_byte(0x02, LCD_CMD, fd_lcd);
}

void cursorLocation(int line, int & fd_lcd) {
    /* Set the cursor location */
    lcd_byte(line, LCD_CMD, fd_lcd);
}

void typeString(const char *s, int & fd_lcd) {
    /* Push the letter 1 by 1 to the screen */
    while (*s) lcd_byte(*(s++), LCD_CHR, fd_lcd);
}


void lcdStart(int & fd_lcd) {
    /* Program to start the LCD (Configuration) */
    lcd_byte(0x33, LCD_CMD, fd_lcd);                // Tells the LCD how much space is available (Down)
    lcd_byte(0x32, LCD_CMD, fd_lcd);                // Tells the LCD how much space is available (Up)
    lcd_byte(0x06, LCD_CMD, fd_lcd);                // The direction the cursor moves to
    lcd_byte(0x0C, LCD_CMD, fd_lcd);                // 0x0F On, now Off
    lcd_byte(0x28, LCD_CMD, fd_lcd);                // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD, fd_lcd);                // Clear display
    usleep(500);
}

void typeFloat(float myFloat, int & fd_lcd) {
    /* This function gets a float and changes it to a char */
    char buffer[20];
    sprintf(buffer, "%4.2f",  myFloat);
    typeString(buffer, fd_lcd);
}

void printPercentage(int & address_lcd, char current_mode, BrickPi3 & BP) {
    /* The function prints the battery percentage on the LCD screen.
     * It also shows the current mode where the robot is in.
     * The function expects a char which represents the current mode L (line), G (grid), F (free)
     */
    float battery = BP.get_voltage_battery();
    float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
    clearLcd(address_lcd);                                          // Clear the lcd
    cursorLocation(LINE1, address_lcd);                             // Set the cursorlocation to line 1
    typeFloat(battery_percentage, address_lcd);                     // Display the battery_percantage
    cursorLocation(LINE2, address_lcd);                             // Set the cursorlocation to line 2
    switch (current_mode){
        case 'L':
            // Prints the string on screen for line follow-mode
            typeString("PCT   Line follow", address_lcd);
            break;
        case 'G':
            // Prints the string on screen for grid navigate
            typeString("PCT   Grid navigate", address_lcd);
            break;
        case 'F':
            // Prints the string on screen for free drive
            typeString("PCT   Free drive", address_lcd);
            break;
        case 'P':
            // Prints the string on screen for line follow-mode with PID
            typeString("PCT   Line PID", address_lcd);
            break;
        case 'R':
            // Prints the string on screen for grid navigate with PID
            typeString("PCT   Grid PID", address_lcd);
        default:
            cout << "ERROR, incorrect input" << endl;
    }
}
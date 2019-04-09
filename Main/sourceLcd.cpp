#include "linefollower.h"

void enableLcd(int bits, int & fd) {
    // Toggle enable pin on LCD display
    usleep(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    usleep(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    usleep(500);
}


void lcd_byte(int bits, int mode, int & fd)   {
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
    enableLcd(bits_high, fd);

    // Low bits
    wiringPiI2CReadReg8(fd, bits_low);
    enableLcd(bits_low, fd);
}

void clearLcd(int & fd) {
    lcd_byte(0x01, LCD_CMD, fd);
    lcd_byte(0x02, LCD_CMD, fd);
}

void cursorLocation(int line, int & fd) {
    lcd_byte(line, LCD_CMD, fd);
}

void typeString(const char *s, int & fd) {
    while (*s) lcd_byte(*(s++), LCD_CHR, fd);
}


void lcdStart(int & fd) {
    // Initialise display
    lcd_byte(0x33, LCD_CMD, fd); // Initialise
    lcd_byte(0x32, LCD_CMD, fd); // Initialise
    lcd_byte(0x06, LCD_CMD, fd); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD, fd); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD, fd); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD, fd); // Clear display
    usleep(500);
}

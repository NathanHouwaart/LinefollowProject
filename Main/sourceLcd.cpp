#include "linefollower.h"

void setLcd(const char message1, const char message2) {
    resetLcd();                           // clear lcd
    lcdLoc(LINE1);                      // set cursor on LINE1
    typeChar(message1);              // Print Calibrating to the lcd
    lcdLoc(LINE2);
    typeChar(message2);
}

void typeFloat(float myFloat) {
// float to string
    char buffer[20];
    sprintf(buffer, "%4.2f",  myFloat);
    typeln(buffer);
}

void typeInt(int i) {
// int to string
    char array1[20];
    sprintf(array1, "%d",  i);
    typeln(array1);
}

void resetLcd(void) {
// Clr lcd and go to home
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

void lcdLoc(int line) {
// Go to a location on the lcd, LINE1 or LINE2
    lcd_byte(line, LCD_CMD);
}

void typeChar(char val) {
// Puts a char at the current posistion
    lcd_byte(val, LCD_CHR);
}


void typeCharArray(const char *s) {
// this allows use of any size string
    while (*s) lcd_byte(*(s++), LCD_CHR);
}

void lcd_byte(int bits, int mode) {
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

void enableLcd(int bits) {
    // Toggle enable pin on LCD display
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    delayMicroseconds(500);
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
#include <wiringPiI2C.h>
#include <wiringPi.h>
// #include <stdlib.h>
#include <stdio.h>
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

void lcd_byte(int bits, int mode);
void enableLcd(int bits);
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line);
void typeCharArray(const char *s);
void typeChar(char val);
void setLcd(const char message1, const char message2);
void resetLcd();
void lcdStart();

int main() {
    if (wiringPiSetup () == -1) {
        cout << "wiringpisetup did not work" << endl << "return 1;" << endl;
        return 1;
    }
    wiringPiValues val;
    val.fd = wiringPiI2CSetup(I2C_ADDR);
    lcdStart(); // setup LCD
    setLcd('test', 'test');
}


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
    typeCharArray(buffer);
}

void typeInt(int i) {
// int to string
    char array1[20];
    sprintf(array1, "%d",  i);
    typeCharArray(array1);
}

void resetLcd() {
// Clr lcd and go to home
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

void lcdLoc(int line) {
// Go to a location on the lcd, LINE1 or LINE2
    lcd_byte(line, LCD_CMD);
}

void typeChar(const char val) {
// Puts a char at the current posistion
    lcd_byte(val, LCD_CHR);
}


void typeCharArray(const char *s) {
// this allows use of any size string
    while (*s) lcd_byte(*(s++), LCD_CHR);
}

void lcd_byte(int bits, int mode) {
    wiringPiValues val;     // Form the struct to acces the fd values
    int bits_high;
    int bits_low;
    // uses the two half byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

    // High bits
    wiringPiI2CReadReg8(val.fd, bits_high);
    enableLcd(bits_high);

    // Low bits
    wiringPiI2CReadReg8(val.fd, bits_low);
    enableLcd(bits_low);
}

void enableLcd(int bits) {
    // Toggle enable pin on LCD display
    wiringPiValues val;     // Form the struct to acces the fd values
    usleep(500);
    wiringPiI2CReadReg8(val.fd, (bits | ENABLE));
    usleep(500);
    wiringPiI2CReadReg8(val.fd, (bits & ~ENABLE));
    usleep(500);
}


void lcdStart() {
    // Initialise display
    lcd_byte(0x33, LCD_CMD); // Initialise
    lcd_byte(0x32, LCD_CMD); // Initialise
    lcd_byte(0x06, LCD_CMD); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD); // Clear display
    usleep(500);
}

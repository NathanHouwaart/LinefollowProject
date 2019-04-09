#include "linefollower.h"

using namespace std;

// TODO: --> Maak loop van de modus freeRideLoop() (Zie activity diagram)

void freeRideLoop(BrickPi3 & BP, int & fd){
    int lcd_counter = 0;
    while (true) {
        lcd_counter++;                      // every loop add 1 one to the counter
        if (lcd_counter >= 5000) {          // after 5000 loops its updates the screen
            float battery = BP.get_voltage_battery();
            float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeString("Freeride", fd);   // print the text on the screen
            cursorLocation(LINE2, fd);     // set the cursorlocation to line 2
            typeFloat(battery_percentage, fd);  // display the battery_percantage
            lcd_counter = 0;                    // reset the counter
        }
    }
}

#include "linefollower.h"

using namespace std;

// TODO: --> Maak loop van de modus freeRideLoop() (Zie activity diagram)

void freeRideLoop(BrickPi3 & BP, int & fd){
    float battery = BP.get_voltage_battery();
    float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
    clearLcd(fd);   // clear the lcd
    cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
    typeString("Grid mode", fd);   // print the text on the screen
    cursorLocation(LINE2, fd);     // set the cursorlocation to line 2
    typeFloat(battery_percentage, fd);  // display the battery_percantage
}

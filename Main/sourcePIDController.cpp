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

int defineError(int avarage_lowest_highest, int dist_lowest_avarage, int dist_highest_avarage, int current_measurment) {
    /* Function calculates how far off the current light measurement is off from the target value.
     * Returns a positive percentage if sensor detects more white.
     * Returns a negative percentage if sensor detects more black.
     */

    float steering_error, current_difference;
    if (current_measurment > avarage_lowest_highest) {
        current_difference = current_measurment - avarage_lowest_highest;
        steering_error = (current_difference / dist_highest_avarage) * 100;
    } else if (current_measurment < avarage_lowest_highest) {
        current_difference = avarage_lowest_highest - current_measurment;
        steering_error = (current_difference / dist_lowest_avarage) * -100;
    } else {
        steering_error = 0;
    }
    return steering_error;
}

void pController(int error_value, BrickPi3 & BP) {
    /*In this function we calculate the speed and the direction for the light sensor*/
    int fine_tune_value = 0;
    unsigned int power = 100;                           // The default value of the variable power is 100
    float direction = (0.01 * error_value) + 1;         // Here we calculate the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {
        // If the direction is higher than 1 and the robot is going right we calculate the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
    } else if (direction < 1) {
        // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
    } else {
        // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    power =fine_tune_value+(power-fine_tune_value)*(limit-fine_tune_value)/(100-fine_tune_value);
    // If the power/ speed is lower than 10 the speed will be 10 to prevent the robot form stalling
    if (power < 10){
        power = 10;
    } else if(power > 100){
        power = 100;
    }
    power = 100-power;
    drive(direction,power,360,BP);                      // We give the direction and the speed to the function drive
}

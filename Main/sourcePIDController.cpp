#include "linefollower.h"

using namespace std;

int defineError(int avarage_lowest_highest, int dist_lowest_avarage, int dist_highest_avarage, int current_measurment) {
    /* Function calculates how far off the current light measurement is off from the target value.
     * Returns a positive percentage if sensor detects more white
     * Returns a negative percentage if sensor detects more black */

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
//    cout << "steering_error: " << steering_error << endl;
//    cout << "Act difference: " << current_difference << endl;
//    cout << "higgest_difference: " << dist_highest_avarage << endl;
//    cout << "laagste verschil: " << dist_lowest_avarage << endl;
    return steering_error;
}

void pController(int error_value, BrickPi3 & BP) {
    /* In this function we calculate the speed and the direction for the light sensor*/
    //TODO: --> wat is lijnwaarde precies? Is nu error_value, en dat is de error ten opzichte van de lijn tussen -100 en 100
    //TODO: -->

    int fine_tune_value = 0;
    unsigned int power = 100;                       // The default value of the variable power is 100
//	cout << error_value << endl;
    float Kp = 0.01;                                           // This is the gain for the p controller. You can adust there for a better experience
    float direction = (Kp * error_value) + 1;     // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                            // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
    } else if (direction < 1) {                       // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
    } else {                                          // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    power =fine_tune_value+(power-fine_tune_value)*(limit-fine_tune_value)/(100-fine_tune_value);
    // If the power/ speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    if (power < 10){
        power = 10;
    } else if(power > 100){
        power = 100;
    }
    power = 100-power;
    drive(direction,power,360,BP); // We give the direction and the speed to the function drive
}

void dController(errorValues & error_values) {
    if (error_values.current_error < 50 && error_values.current_error > -50) {
        int Kd = 1;             // This is used as a time unit within the d_error calculation
        int d_error = (error_values.current_error - error_values.last_error) / Kd;
        int adjusted_error = error_values.current_error + d_error;
        error_values.last_error = error_values.current_error;
        error_values.current_error = adjusted_error;    
    }  
}

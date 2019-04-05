#include "linefollower.h"

using namespace std;

float defineError(int avarage_lowest_highest, int dist_lowest_avarage, int dist_highest_avarage, int current_measurment) {
    /* Function calculates how far off the current light measurement is off from the target value.
     * Returns a positive percentage if sensor detects more white
     * Returns a negative percentage if sensor detects more black */

    float steering_error, current_difference;
    if (current_measurment > avarage_lowest_highest) {
        current_difference = current_measurment - avarage_lowest_highest;
        steering_error = (current_difference / dist_highest_avarage) * 100.0;
    } else if (current_measurment < avarage_lowest_highest) {
        current_difference = avarage_lowest_highest - current_measurment;
        steering_error = (current_difference / dist_lowest_avarage) * -100.0;
    } else {
        steering_error = 0.0;
    }
//    cout << "steering_error: " << steering_error << endl;
//    cout << "Act difference: " << current_difference << endl;
//    cout << "higgest_difference: " << dist_highest_avarage << endl;
//    cout << "laagste verschil: " << dist_lowest_avarage << endl;
    return steering_error;
}

void pController(float error_value, PIDValues & pidValues) {
    /* In this function we calculate the speed and the direction for the light sensor*/
    //TODO: --> wat is lijnwaarde precies? Is nu error_value, en dat is de error ten opzichte van de lijn tussen -100 en 100
    //TODO: -->

    int from_range_min = 0;
    int to_range_min = 0;
    unsigned int power = 100;                       // The default value of the variable power is 100
    float Kp = 0.005;                                           // This is the gain for the p controller. You can adust there for a better experience
    float direction = (Kp * error_value) + 0.5;     // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                            // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
    } else if (direction < 1) {                       // Here does it the same for right but now it goes left
        power = direction * 100;
    } else {                                          // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    power = to_range_min+(power-from_range_min)*(limit-to_range_min)/(100-from_range_min);
    // If the power/ speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    if (power < 10){
        power = 10;
    } else if(power > 100){
        power = 100;
    }
    pidValues.power = power;
    pidValues.p_control = direction;
}

void dController(errorValues & error_values, PIDValues & pidValues) {
    if (error_values.counter == error_values.Kd) {                      // Wait a couple of measurements to calculate the D
        float d_error = (error_values.current_error - error_values.last_error) / error_values.Kd;      // this calculates the error
        if (d_error > 100) {                    // To chatch any invaild values
            d_error = 100;
        } else if (d_error < -100) {
            d_error = -100;
        }

        cout << "last error: " << error_values.last_error << endl;
	    cout << "current error: " << error_values.current_error << endl;
	    cout << "Kd: " << error_values.Kd << endl;

	    pidValues.d_control = (0.05 * d_error) + 0.5;                              // Put variable in a struct to use it in oter functions
        error_values.last_error = error_values.current_error;       // Define the last erre
        error_values.counter = 0;
    }
    error_values.counter++;                                         // Here we add 1 to the counter to keep track how many measurements has taken place
}

void pdControl(PIDValues & pidValues, BrickPi3 & BP){
    /* In this function we combine the P and the D controller and pass it on to the drive function */
    float final_control = pidValues.p_control + pidValues.d_control;
    cout << "p waarde: " << pidValues.p_control << endl;
    cout << "d waarde: " << pidValues.d_control << endl;
    cout << "stuurwaarde: " << final_control << endl;
    drive(final_control, 100, pidValues.max_speed, BP);
}
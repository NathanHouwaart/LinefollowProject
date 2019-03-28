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

void speedLimiter(int & right, int & left, const int & maximum_speed = 360) {
    /* This is the limiter function.
     * This function makes sure that the motors don't go above the maximum_speed.
     * It checks if the given motorspeed is above the max, and if it calculates the difference.
     * And retracts this of both motors.
     * We decided to use 2 ifs because then we are sure that none of the motors are going faster.
     */
    if (right > maximum_speed) {
        int verschil_rechts = right - maximum_speed;    //Calculates difference
        right -= verschil_rechts;                       //subtracs the difference from both motors.
        left -= verschil_rechts;
        if(left < 0){                                   //makes sure that the other motor doesn't go 0.
            left = 0;
        }
    }
    if (left > maximum_speed) {
        int verschil_links = left - maximum_speed;      //The same principles apply here as seen above.
        right -= verschil_links;
        left -= verschil_links;
        if(right < 0){
            right = 0;
        }
    }
}

void MotorController(int left, int right, BrickPi3 & BP) {
    /* This function controls the motors.
     * First it sends the given motor speed setting through the limiter
     * After that it sets the motor to the given speed.
     */
    uint8_t motor_right = PORT_D;                       // Here we define the motor ports so we can talk to them
    uint8_t motor_left = PORT_A;
    speedLimiter(right, left, MAX_MOTOR_SPEED);         // The values of the right and left motor are send to the limiter
    BP.set_motor_dps(motor_right, right);               // This starts the motors
    BP.set_motor_dps(motor_left, left);
}

void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, BrickPi3 & BP){
    /*
     * Direction control is either -2, -1 or a value between 0 and 2.
     * If the direction control is 1 the robot will go straight.
     * If direction control = -1, the robot stops, if direction control = -2, the robot reverses.
     * If direction < 1 and > 0 steer left. > 1 < 2 steer right.
     * speed_calculator is a value between 0 and 100 that is used to set the motor_speed
     * The max_motor_speed is the base value of the motors after calculating with speed_calculator.
     * The base_speed is the value that sets the max speed.
     */
    if(direction_control < -2 || direction_control > 2){
        cout << "Please give a direction control between -2 and 2." << endl;
    } else if(speed_multiplier_percentage < 0 || speed_multiplier_percentage > 100){
        cout << "Please give a speed_calculator between 0 and 100." << endl;
    } else{
        int motor_speed = rotation_speed*(speed_multiplier_percentage/100.0);   // Calculates motor rotation speed
        int motor_speed_L;                                                      // Variables to save motor speed of both engines
        int motor_speed_R;
        if (direction_control >= 0){                                            // && direction_control <= 2){ ---kan ik als het goed is weglaten!---
            motor_speed_L = motor_speed * direction_control;                    // To steer, one engine has to provide more power than the other
            motor_speed_R = motor_speed * (2-direction_control);
        } else if (direction_control == -1) {                                   // Robot stops
            motor_speed_L = 0;
            motor_speed_R = 0;
        } else if (direction_control == -2) {                                   // Robot will go backwards
            motor_speed_L = -1 * motor_speed;
            motor_speed_R = -1 * motor_speed;
        } else{
            cout << "The given value doesn't correspond to the given parameters of x=-2,-1 or 0=<x<=2." << endl;
        }
        //cout << "speed_calculator: " << speed_calculator << " motor_speed: " << motor_speed << " motor L:" << motor_speed_L << " motor R: " << motor_speed_R << endl;
        MotorController(motor_speed_L, motor_speed_R, BP);
    }

}

bool checkVoltage(BrickPi3 & BP) {
    /*This fuction looks for the battery volage to make sure the battery is not empty and we do not damage it*/
    if (BP.get_voltage_battery() < 11.0) {              // Checks if battery is empty
        cout << "Battery empty, charge" << endl;
        return false;
    } else {
        cout << BP.get_voltage_battery() << endl;
        cout << BP.get_voltage_9v() << endl;            // If the battery is not empty the battery voltages will displayd in the console
        cout << BP.get_voltage_5v() << endl;
        cout << BP.get_voltage_3v3() << endl;
        return true;
    }
}

bool lightsensorBlackLineDetect(sensor_light_t  & Light1, BrickPi3 & BP) {
    if (BP.get_sensor(PORT_3, Light1) == 0) {
        return (Light1.reflected >= 2400);
    }
}

bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP) {
    /* Function checks if color sensor on port 3 (Hardcoded) has detected a black line.
     * If color == black it returns true.
     * Otherwise it returns false */
    // TODO: Check if return false can only be at case 6.

    if (BP.get_sensor(PORT_1, Color1) == 0) {
        int color_rgb_sensor = Color1.color;
        switch (color_rgb_sensor) {
            case 1:
                return true;
            case 2:
                return false;
            case 3:
                return false;
            case 4:
                return false;
            case 5:
                return false;
            case 6:
                return false;
        }
    }
    return false;
}

void stuur(int lijn_waarde, BrickPi3 & BP) {
    /* In this function we calculate the speed and the direction for the light sensor*/
    //TODO: wat is lijnwaarde precies?

    int fine_tune_value = 0;
    unsigned int power = 100;                       // The default value of the variable power is 100

    float direction = (0.01 * lijn_waarde) + 1;     // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                            // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
        if (power > 100) power = 100;
    }
    else if (direction < 1) {                       // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
        if (power > 100) power = 100;
    }
    else {                                          // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    power =fine_tune_value+(power-fine_tune_value)*(limit-fine_tune_value)/(100-fine_tune_value);
    // If the power/ speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    if (power < 10)
        power = 10;
    power = 100-power;
    drive(direction,power,360,BP); // We give the direction and the speed to the function drive
}

vector<int> defineDifferenceToAverage(const int & min_reflection_value , const int & max_reflection_value) {
    /* In order to calculate how far off the current measurement is from our target, we first need to define the target value.
     * This is done by calculating the average of the min and max reflection value.
     * Then the difference from minimum to avg & maximum to avg is calculated.
     * These values are used to determine how far the current measurement is off from our target on a scale of 0 - 100 %. */

    vector<int> calculated_distance_to_average;                                 // Constructs a vector to return multiple values
    int average = (max_reflection_value + min_reflection_value) / 2;            // Calculate average
    int distance_lowest_value_to_avg = average - min_reflection_value;        // Calculate distance min--avg
    int distance_highest_value_to_avg = max_reflection_value - average;       // Calculate distance max--avg

    calculated_distance_to_average.push_back(average);
    calculated_distance_to_average.push_back(distance_lowest_value_to_avg);
    calculated_distance_to_average.push_back(distance_highest_value_to_avg);

    return calculated_distance_to_average;                                      // Return values
}

int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting) {
    /* Function calculates how far off the current light measurement is off from the target value.
     * Returns a positive percentage if sensor detects more white
     * Returns a negative percentage if sensor detects more black */

    float stuurwaarde, actueel_verschil;
    // TODO --> tussentijdse  hoogste & laagste waardes veranderen. Dit met bepaalAfwijking
    // TODO --> Check float stuurwaarde and function return

    if (actuele_licht_meting > gemiddelde) {
        actueel_verschil = actuele_licht_meting - gemiddelde;
        stuurwaarde = (actueel_verschil / hoogste_verschil) * 100;
    } else if (actuele_licht_meting < gemiddelde) {
        actueel_verschil = gemiddelde - actuele_licht_meting;
        stuurwaarde = (actueel_verschil / laagste_verschil) * -100;
    } else {
        stuurwaarde = 0;
    }
    return stuurwaarde;
}


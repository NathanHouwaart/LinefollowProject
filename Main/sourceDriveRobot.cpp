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

void speedLimiter(int & right, int & left, const int & maximum_speed) {
    /* This is the limiter function.
     * This function makes sure that the motors don't go above the maximum_speed.
     * It checks if the given motorspeed is above the max, and if it calculates the difference.
     * And retracts this of both motors.
     * We decided to use 2 ifs because then we are sure that none of the motors are going faster.
     */
    if (right > maximum_speed) {
        // right motor above maximum_speed so correct values
        int difference_right = right - maximum_speed;    //Calculates difference
        right -= difference_right;                       //subtracts the difference from both motors.
        left -= difference_right;
        if(left < 0){
            // Makes sure that the other motor doesn't go beneath zero
            left = 0;
        }
    }
    if (left > maximum_speed) {
        // left motor above maximum_speed so correct values
        int difference_left = left - maximum_speed;      //The same principles applies here as seen above.
        right -= difference_left;
        left -= difference_left;
        if(right < 0){
            // Makes sure that the other motor doesn't go beneath zero
            right = 0;
        }
    }
}

void MotorController(int left, int right, BrickPi3 & BP) {
    /* This function controls the motors.
     * First it sends the given motor speed setting through the limiter
     * After that it sets the motor to the given speed.
     */
    uint8_t motor_right = PORT_D;                       // Here we define the motor ports so we can communicate
    uint8_t motor_left = PORT_A;                        // Here we define the motor ports so we can communicate
    speedLimiter(right, left, MAX_MOTOR_SPEED);         // The values of the right and left motor are send to the limiter
    BP.set_motor_dps(motor_right, right);               // Starts the right motor
    BP.set_motor_dps(motor_left, left);                 // Starts the left motor
}

void MotorControllerPower(int left, int right, BrickPi3 & BP) {
    /* This function controls the motors.
     * First it sends the given motor speed setting through the limiter
     * After that it sets the motor to the given speed.
     */
    uint8_t motor_right = PORT_A;                       // Here we define the motor ports so we can talk to them
    uint8_t motor_left = PORT_D;
    //speedLimiter(right, left, MAX_MOTOR_SPEED);         // The values of the right and left motor are send to the limiter
    int right_power = right * 127 / 100;
    int left_power = left * 127 / 100;
    if(right_power > 127) right_power = 127;
    else if (right_power < -127) right_power = -127;
    if(left_power > 127) left_power = 127;
    else if (left_power < -127) left_power = -127; 
    cout << "LEFT POWER: " << left_power << endl;
    cout << "RIGHT POWER: " << right_power << endl;

    BP.set_motor_power(motor_right, right_power);
    BP.set_motor_power(motor_left, left_power);
}

void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, BrickPi3 & BP){
    /* Direction control is either -2, -1 or a value between 0 and 2.
     * If the direction control is 1 the robot will go straight.
     * If direction control = -1, the robot stops, if direction control = -2, the robot reverses.
     * If direction < 1 and > 0 steer left. > 1 < 2 steer right.
     * speed_calculator is a value between 0 and 100 that is used to set the motor_speed
     * The max_motor_speed is the base value of the motors after calculating with speed_calculator.
     * The base_speed is the value that sets the max speed.
     */
    if(direction_control < -2 || direction_control > 2){
        // The given direction_control is not according to the expectations, print error
        cout << "ERROR, Please give a direction control between -2 and 2." << endl;
    } else if(speed_multiplier_percentage < 0 || speed_multiplier_percentage > 100){
        // The given speed_multiplier_percentage is not according to expectations, print error
        cout << "ERROR, Please give a speed_calculator between 0 and 100." << endl;
    } else{
        int motor_basespeed = rotation_speed*(speed_multiplier_percentage/100.0);   // Calculates motor rotation speed
        int motor_speed_L;                                                          // Variables to save motor speed of both engines
        int motor_speed_R;
        if (direction_control >= 0){
            // Robot needs to steer, one engine has to turn harder
            motor_speed_L = motor_basespeed * direction_control;                    // To steer, one engine has to provide more power than the other
            motor_speed_R = motor_basespeed * (2-direction_control);                // Calculate the motor_speed settings
        } else if (direction_control == -1) {
            // Robot stops
            motor_speed_L = 0;                                                      // Motor_speed needs to be 0
            motor_speed_R = 0;
        } else if (direction_control == -2) {
            // Robot will go backwards
            motor_speed_L = -1 * motor_basespeed;                                   // Robot backwards so speed needs to be negative
            motor_speed_R = -1 * motor_basespeed;
        } else{
            // The given direction_control is < 0 and is not -1 or -2 so it is incorrect
            cout << "The given value doesn't correspond to the given parameters of x=-2,-1 or 0=<x<=2." << endl;
        }
        MotorController(motor_speed_L, motor_speed_R, BP);                          // The calculated is send where the motor is controlled
    }
}

void driveOnSpot(char turn_direction, BrickPi3 & BP){
    /* This function is used to turn 90 degrees on the spot.
     * The function expects you give a char, this corresponds to L or R.
     */
    uint8_t motor_right = PORT_D;                                           // Setting the motor port to communicate with motor
    uint8_t motor_left = PORT_A;                                            // Setting the motor port to communicate with motor
    int32_t turn_90_degrees = 225;                                          // The degrees the motors need to turn for 90 turning on axis
    if(turn_direction != 'L' && turn_direction != 'R'){
        // Input is incorrect
        cout << "Nee ik wil een L of een R!" << endl;
    }else if(turn_direction == 'L'){
        // The robot needs to turn 90 degrees to the left
        BP.set_motor_position_relative(motor_left,(-1*turn_90_degrees));    // To left so left motor needs to go backwards
        BP.set_motor_position_relative(motor_right,turn_90_degrees);        // To left so right motor needs to go forward
    }else{
        // The robot needs to turn 90 degrees to the right
        BP.set_motor_position_relative(motor_left,turn_90_degrees);         // Same principles only switched the motors
        BP.set_motor_position_relative(motor_right,(-1*turn_90_degrees));
    }
    usleep(1000*500);                                                       // Give the robot time to turn before continuing
}

void MotorControllerTurn(const int & turn, const int & target_power, const float & turn_modifier, BrickPi3 & BP){
    /* Function mimics the function of the steer block in the nxt programming language */
    int turn_value = turn;
    uint8_t motor_right = PORT_D;
    uint8_t motor_left = PORT_A;
    int speedD, speedA;
    if (turn_value > 100) turn_value = 100;
    else if(turn_value < -100) turn_value = -100;
    if (turn_value > 0) {                                         // If steer > 0
        speedD = target_power;                                  // Motor D needs to be at target speed
        speedA = target_power + turn_value * turn_modifier;            // Motor A needs to slow down. Stops at 50, reverses at 100
    } else if (turn_value < 0) {                                  // If steer < 0
        speedA = target_power;                                  // Motor A needs to be at target power
        speedD = target_power - turn_value * turn_modifier;            // Motor D needs to slow down. stops at -50, reverses at -100;
    } else {                                                // If steer == 0
        speedA = target_power;                                  // Go target power forward
        speedD = target_power;
    }
    if (speedA < -100) speedA = -100;
    else if(speedA > 100) speedA = 100;                       // Protection to not overload motor power;
    if (speedD < -100) speedD = -100;
    else if(speedD > 100) speedD = 100;

    cout << "SPEED A: " << speedA << endl;                  // Debug only
    cout << "SPEED D: " << speedD << endl;

    vector<int> motor_speeds = convertPowerValues(speedA, speedD);   // Convert power values to different scale
    BP.set_motor_power(motor_right, motor_speeds[1]);                // Sets motor power of motor D accordingly
    BP.set_motor_power(motor_left, motor_speeds[0]);                 // Sets motor power of motor A accordingly

}

vector<int> convertPowerValues(const int & speedA, const int & speedD){
    /* Function translates power on a scale from 0 - 100 to a scale form 0 - 127 */
    int right_power = speedD * 127 / 100;                       // Formula to convert to new scale
    int left_power = speedA * 127 / 100;
    cout << "LEFT POWER: " << left_power << endl;
    cout << "RIGHT POWER: " << right_power << endl;
    vector<int> motor_speeds = {right_power, left_power};
    return motor_speeds;
}

/* --------------------------------------------------------------------------------------------- -----Created by TICT-1C groep 1 ----- -----Lego mindstorms linefollower robot 
project ----- -----Contributors: ----- -----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart -----
 ---------------------------------------------------------------------------------------------
*/

#include "linefollower.h"

using namespace std;
vector<float> buffer_amount_of_aggression ={};
bool sharp_corner = false;
float amount_of_aggression = 0;
float sharp_corner_white_aggression = 0; //bepaald hoe hard het andere wiel achteruit gaat
float sharp_corner_black_aggression = 0;

 
// TODO: --> turn sharp functie

void speedLimiter(int & right, int & left, const int & maximum_speed) {
    /* This is the limiter function.
     * This function makes sure that the motors don't go above the maximum_speed.
     * It checks if the given motorspeed is above the max, and if it calculates the difference.
     * And retracts this of both motors.
     * We decided to use 2 ifs because then we are sure that none of the motors are going faster.
     */
    if (right > maximum_speed) {
        int difference_right = right - maximum_speed;    //Calculates difference
        right -= difference_right;                       //subtracs the difference from both motors.
        left -= difference_right;
        if(left < 0 && !sharp_corner){                                   //makes sure that the other motor doesn't go 0.
        	left = 0;
        }
    }
    if (left > maximum_speed) {
        int difference_left = left - maximum_speed;      //The same principles apply here as seen above.
        right -= difference_left;
        left -= difference_left;
        if(right < 0 && !sharp_corner){
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

void drive(float direction_control, unsigned int speed_multiplier_percentage, unsigned int rotation_speed, SharpCornerSettings & SharpCorner, BrickPi3 & BP){
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
        cout << "direction_control: " << direction_control << endl;
        int motor_speed = rotation_speed*(speed_multiplier_percentage/100.0);   // Calculates motor rotation speed
        int motor_speed_L;                                                      // Variables to save motor speed of both engines
        int motor_speed_R;
        size_t buffer_size = 3;

        if(buffer_amount_of_aggression.size() < buffer_size){
            buffer_amount_of_aggression.push_back(direction_control);
        } else{
            buffer_amount_of_aggression.push_back(direction_control);
            amount_of_aggression = vectorAvarage(buffer_amount_of_aggression);
            buffer_amount_of_aggression = {};
            if(amount_of_aggression > 1){
                sharp_corner_white_aggression = ((direction_control-SharpCorner.min_value_white)/SharpCorner.min_value_white)*SharpCorner.max_aggression_white;
                sharp_corner_black_aggression = 0;
            } else{
                sharp_corner_white_aggression = 0;
                sharp_corner_black_aggression = ((direction_control-SharpCorner.min_value_black)/SharpCorner.min_value_black)*SharpCorner.max_aggression_black;
            }
        }

        //Bepaald of een corner een sharp corner is.
        //TODO: Willen we dit echt een directe value? Geen kleine buffer
        sharp_corner = (direction_control > SharpCorner.min_value_white || direction_control < SharpCorner.min_value_black);


        if (direction_control >= 0){                                            // && direction_control <= 2){ ---kan ik als het goed is weglaten!---
	        turnUS(direction_control,BP);
            motor_speed_L = motor_speed * direction_control;                    // To steer, one engine has to provide more power than the other
            motor_speed_R = motor_speed * (2-direction_control);

            if(sharp_corner && direction_control < 1){
                motor_speed_L = (motor_speed_R*sharp_corner_black_aggression);
            } else if(sharp_corner && direction_control > 1){
                motor_speed_R = (motor_speed_L*sharp_corner_white_aggression);
            }
        } else if (direction_control == -1) {                                   // Robot stops
            motor_speed_L = 0;
            motor_speed_R = 0;
        } else if (direction_control == -2) {                                   // Robot will go backwards
            motor_speed_L = -1 * motor_speed;
            motor_speed_R = -1 * motor_speed;
        } else{
            cout << "The given value doesn't correspond to the given parameters of x=-2,-1 or 0=<x<=2." << endl;
        }
	if(sharp_corner){
        	cout << "amount_of_aggression: " << amount_of_aggression << endl;
        	cout << "sharp_corner: " << sharp_corner << endl;
        	cout << "motor_speed_L: " << motor_speed_L << endl;
        	cout << "motor_speed_R: " << motor_speed_R << endl;
        	cout << "white_aggression: " << sharp_corner_white_aggression << endl;
            cout << "black_aggression: " << sharp_corner_black_aggression << endl;
	}
        MotorController(motor_speed_L, motor_speed_R, BP);
    }
}

void driveOnSpot(char turn_direction, BrickPi3 & BP){
    /* This function is used to turn 90 degrees on the spot.
     * The function expects you give the turn side.
    */
    uint8_t motor_right = PORT_D;
    uint8_t motor_left = PORT_A;
    int32_t turn_90_degrees = 250;
    if(turn_direction != 'L' && turn_direction != 'R'){
        cout << "Nee ik wil een L of een R!" << endl;
    }else if(turn_direction == 'L'){
        BP.set_motor_position_relative(motor_left,(-1*turn_90_degrees));
        BP.set_motor_position_relative(motor_right,turn_90_degrees);
    }else{
        BP.set_motor_position_relative(motor_left,turn_90_degrees);
        BP.set_motor_position_relative(motor_right,(-1*turn_90_degrees));
    }
    usleep(1000*800);
}


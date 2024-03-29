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

/*-----Declaration of global variables-----*/
int current_position_us= 0;						// Saves the orientation of the ultrasonic sensor
vector<float> buffer_motor_values = {};		    // The buffer for turning the ultrasonic sensor

float vectorAverage(const vector<float> & to_calculate){
	/* This function calculates the average of the given vector.
	 * It expects a vector with floats, it returns a float.
	 */
	float sum = 0;
	int total = 0;
	for(float i : to_calculate){
		sum += i;
		total++;
	}
	return sum/total;
}

//WARNING THIS FUNCTION IS NOT USED
void turnUS (float values_wheels, BrickPi3 & BP){
	/* The motor turns the Ultrasonic sensor.
	 * This function gets a value(float) between 0 en 2. (0: 90 degrees left, 1: 0 degrees(forward) 2: 90 degrees right)
	 * The sensor turns with the road so it can detect an object while steering.
	 * Then this function calculates the degrees to turn with the old calculations.
	 * So it knows where the US sensor is looking at(degrees) and calculates the new value.
	 * The difference between those values, is the turn.
	 */

	int min_degrees_turned = -95; 				// 5 degrees buffer because the motor is not accurate all the time
	int max_degrees_turned = 95; 				// 5 degrees buffer because the motor is not accurate all the time
	int target_degrees_us = 0;					// Variable to save the next orientation of the US sensor
	int degrees_to_turn = 0;					// Variable to save the amount of degrees the US sensors needs to turn
	int buffersize = 0; 						// Sets buffersize, smaller buffer, faster reaction orientation (300)
	uint8_t motor_middle = PORT_B; 				// Setting the motor port to communicate with motor

	if(buffer_motor_values.size() < buffersize){
		// Add one measurement to the buffer(vector)
		buffer_motor_values.push_back(values_wheels);
	} else{
		// Add the last measurement to the buffer
		buffer_motor_values.push_back(values_wheels);
		float average_of_vector = vectorAverage(buffer_motor_values); 			// Calculate the average of buffer
		buffer_motor_values = {}; 												// Clears buffer
		if(average_of_vector > 1){
			// Orientation of US sensor is to right
			target_degrees_us = (min_degrees_turned * (average_of_vector-1));   //calculate how far the US needs to turn right
		} else if(average_of_vector < 1){
			// Orientation of US sensor is to left
			target_degrees_us = max_degrees_turned * (1-average_of_vector); 	//calculate how far the US needs to turn left
		}

		degrees_to_turn = target_degrees_us - current_position_us; 			    // Calculate the difference
		current_position_us = target_degrees_us; 								// Set the new position
		BP.set_motor_position_relative(motor_middle, degrees_to_turn); 		    // Turn the motor
	}
}

void steeringRobot(char to_steer, BrickPi3 & BP){
	/* This function is called when the robot is driving around the object
	 * The function reacts on the given input, this is either going straight on, steer left or right.
	 * The input can be F, L or R. The motors are controlled with set_motor_dps
	 */
	uint8_t motor_left = PORT_A;						// Setting the motor port to communicate with motor
	uint8_t motor_right = PORT_D;						// Setting the motor port to communicate with motor
	int16_t dps_motor = 200;							// The base speed of the motors
	float steering_factor_high = 1.6;					// Factor for steering robot, motor opposite of steering direction needs to be higher
	float steering_factor_low = 0.7;					// Factor for steering robot, motor steering direction needs to be lower

	if(to_steer == 'F'){
		// Robot goes straight
		BP.set_motor_dps(motor_left, dps_motor);
		BP.set_motor_dps(motor_right, dps_motor);
	} else if(to_steer == 'L'){
		// Robot steers to left
		BP.set_motor_dps(motor_left, dps_motor * 0.7);
		BP.set_motor_dps(motor_right, dps_motor * 1.6);
	} else if(to_steer == 'R'){
		// Robot steers to right
		BP.set_motor_dps(motor_left, dps_motor * 1.6);
		BP.set_motor_dps(motor_right, dps_motor * 0.7);
	} else{
		// Input is wrong so stop robot
		BP.set_motor_dps(motor_left, 0);
		BP.set_motor_dps(motor_right, 0);
	}
}

void driveAroundObject(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP){
	/* The function is called when the robot needs to drive around the object
	 * It turns the robot, calculates the target distance between the obstacle and US
	 * It goes in a loop where we determine if the robot needs to go left, forward or right
	 * We stay in the while if both color sensors see above the average(white)
	 *
	 * After the first loop, we have a loop that makes sure that both sensors see the line.
	 * if both sensors are on the line we let the robot cross the line and turn it to right.
	 */

	uint8_t sensor_us = PORT_4;							// Setting the sensor port to communicate with ultrasonic sensor
	uint8_t sensor_color1 = PORT_1;						// Setting the sensor port to communicate with color sensor
	uint8_t sensor_color2 = PORT_3;						// Setting the sensor port to communicate with color sensor
	uint8_t motor_left = PORT_A;						// Setting the motor port to communicate with motor
	uint8_t motor_right = PORT_D;						// Setting the motor port to communicate with motor
	BP.set_motor_limits(motor_left,60,200);			    // Setting the motor limit when using set_motor_position_relative
	BP.set_motor_limits(motor_right,60,200);			// Setting the motor limit when using set_motor_position_relative

	driveOnSpot('R',BP); 								// Turn robot to the right, on his axis.
	usleep(1000*1000);									// Makes sure the robot is turned all te way before next step
	turnUS(0,BP); 										// Turns the US sensor, it makes sure it is turned 90 degrees to the left on the driving direction
	usleep(1000*1000);									// Makes sure the US sensor is turned correct before measuring

	getUltraSValue(sensor_us, UltraSonic, BP);			// Update the data struct of ultrasonic sensor
	BP.get_sensor(sensor_color1, Color1);				// Update the data struct of color sensor 1
	BP.get_sensor(sensor_color2, Color2);				// Update the data struct of color sensor 2

	int target_distance = UltraSonic.cm;				// Sets the target_distance between object and US sensor/ robot
	int current_distance = UltraSonic.cm;				// Sets the current distance between object and US sensor/ robot

	if(target_distance > 50){
		// Makes sure if the US sensor has a wrong measurement it has a reasonable target_distance
		target_distance = 15;
	}

	cout << "YEEBUG: target_distance: " << target_distance << endl;
	
	/*-----While keeps going until either color1 or color2 detects a black line-----*/
	while(Color1.reflected_red > average_black_line && Color2.reflected_red > average_black_line){
		if(current_distance > 200){
			// If the distance is 255 the code doesn't work so set current_distance to a value that works
			current_distance = 200;
		}

		if(current_distance < target_distance){
			// The object is closer than target, so steer to right (away from target)
			steeringRobot('R', BP);
		} else if(current_distance > target_distance){
			// The object is furhter away than target, so steer to left (to target)
			steeringRobot('L', BP);
		} else{
			// The object is the correct distance, so go straight on
			steeringRobot('F', BP);
		}

		getUltraSValue(PORT_4, UltraSonic, BP); 		// Update the data struct of ultrasonic sensor
		current_distance = UltraSonic.cm;				// Update the current_distance
		BP.get_sensor(PORT_1, Color1);					// Update the data struct of color sensor 1
		BP.get_sensor(PORT_3, Color2);					// Update the data struct of color sensor 2
	}

	float dps_basespeed = 150;							// Base dps rotation for motors
	float factor_backwards = -1;
	
	cout << "YEEBUGING: Ben bij dps_basespeed " << endl;
    /* The while is true if one of the 2 color sensors, see white (greater than average)
	 * The while loops tries to line-up both sensors with the line */
	while (Color1.reflected_red > average_black_line || Color2.reflected_red > average_black_line){
		BP.get_sensor(PORT_1, Color1);					// Update the data struct of color sensor 1
		BP.get_sensor(PORT_3, Color2);					// Update the data struct of color sensor 2

	    if(Color1.reflected_red < average_black_line && Color2.reflected_red > average_black_line){
	    	// Turns the robot to the right on axis, so color2 also sees the line
	        BP.set_motor_dps(motor_left, (factor_backwards*dps_basespeed));
	        BP.set_motor_dps(motor_right,dps_basespeed);
	    } else if(Color1.reflected_red > average_black_line && Color2.reflected_red < average_black_line){
			// Turns the robot to the left on axis, so color1 also sees the line
			BP.set_motor_dps(motor_left, dps_basespeed);
            BP.set_motor_dps(motor_right,(factor_backwards*dps_basespeed));
	    }
	}

	// The following function-calls make the robot cross the line and make it turn
	crossLine(90, BP);									// Robot crosses the line
	usleep(1000*500);									// Sleep makes sure the robot crosses the line
	driveOnSpot('R', BP);								// Turn robot to the right on the axis
	usleep(1000*500);									// Sleep makes sure the robot has turned on spot
	turnUS(1,BP);										// Turn ultrasonic sensor straight forward
	usleep(1000*200);									// Again sleep to make sure US sensor is turned back
	BP.set_motor_limits(motor_left,127,800);			// Remove the limiter, makes sure normal program has no limit
	BP.set_motor_limits(motor_right,127,800);			// Remove the limiter, makes sure normal program has no limit
}

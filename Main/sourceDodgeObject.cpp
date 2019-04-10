#include "linefollower.h"

using namespace std;

int current_position_us=0;
vector<float> buffer_motor_values = {};

float vectorAvarage(const vector<float> & to_calculate){
	/*
	 * This function calculates the average of the given value
	 */
	float sum = 0;
	int total = 0;
	for(float i : to_calculate){
		sum += i;
		total++;
	}
	return sum/total;
}

void turnUS (float values_wheels, BrickPi3 & BP){
	/*
	 * The motor turns the Ultrasonic sensor.
	 * This function gets a value(float) between 0 en 2. (0: 90 degrees left, 1: 0 degrees(forward) 2: 90 degrees right)
	 * The sensor turns with the road so it can detect an object while steering.
	 * Then this function calculates the degrees to turn with the old calculations.
	 * So it knows where the US sensor is looking at(degrees) and calculates the new value.
	 * The difference between those values, is the turn.
	 */
	//values_wheels een waarde tussen 0 & 2
	int max_degrees_turned = -95; //5 degrees buffer because the motor is not accurate all the time
	int min_degrees_turned = 95; //5 degrees buffer because the motor is not accurate all the time
	int target_degrees_us = 0;
	int degrees_to_turn = 0;
	uint8_t motor_middle = PORT_B; // Setting the motor to communicate
	int buffersize = 0; //Moet 300 zijn
	if(buffer_motor_values.size() < buffersize){
		buffer_motor_values.push_back(values_wheels); //add one measurement to the  buffer(vector)
	} else{
		buffer_motor_values.push_back(values_wheels); //If buffer is full (bigger then buffersize)
		float avarage_of_vector = vectorAvarage(buffer_motor_values); //calculate average
		buffer_motor_values = {}; //clean buffer for next object
		if(avarage_of_vector == 1){ // look forward
			target_degrees_us = 0;
		} else if(avarage_of_vector < 1){ // look left
			target_degrees_us = min_degrees_turned * (1-avarage_of_vector); //calculate how far the US needs to turn left
		} else{ //look right
			target_degrees_us = (max_degrees_turned * (avarage_of_vector-1)); //calculate how far the US needs to turn right
		}
		degrees_to_turn = target_degrees_us - current_position_us; // Calculate the difference
		current_position_us = target_degrees_us; // Set the new position for the next measurement
		BP.set_motor_position_relative(motor_middle, degrees_to_turn); //Turn the motor
	}
}

void steeringRobot(char to_steer, BrickPi3 & BP){
	/*
	 * This function is called when the robot is driving around the object
	 * The function reacts on the given input, this is either going straight on, steer left or right.
	 */
	uint8_t motor_left = PORT_A;
	uint8_t motor_right = PORT_D;
	int32_t motor_rotation = 400;
	float steering_factor = 0.8; // The amount that the robot steers
	int16_t dps_motor = 200;
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
		BP.set_motor_dps(motor_left, dps_motor*0);
	}
}

void driveAroundObject(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP){
	uint8_t motor_left = PORT_A;
	uint8_t motor_right = PORT_D;
	BP.set_motor_limits(motor_left,60,200);
	BP.set_motor_limits(motor_right,60,200);

	cout << "YEEBUG: turn robot on spot." << endl;
	driveOnSpot('R',BP); // Turn robot to the right, on his axis.
	usleep(1000*1500);
	turnUS(0,BP); // Turns the US sensor, it makes sure it is turned 90 right-angled on the driving direction
	usleep(1000*2000);

	getUltraSValue(PORT_4, UltraSonic, BP);
	int target_distance = UltraSonic.cm;
	int current_distance = UltraSonic.cm;
	BP.get_sensor(PORT_1, Color1);
	BP.get_sensor(PORT_3, Color2);
	if(target_distance > 100){
		target_distance = 15;
	}
	cout << "YEEBUG: target_distance: " << target_distance << endl;
	//Go straight until one of the color sensors detects a black line.
	while(Color1.reflected_red > average_black_line && Color2.reflected_red > average_black_line){
//		cout << "current_distance: " << current_distance << endl;
		if(current_distance > 200){
			current_distance = 200;
//			cout << "Ik ben in de if: " << current_distance << endl;
		}
//		cout << "target_distance: " << target_distance << endl;

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
		getUltraSValue(PORT_4, UltraSonic, BP);
		current_distance = UltraSonic.cm;
		BP.get_sensor(PORT_1, Color1);
		BP.get_sensor(PORT_3, Color2);
	}
	float dps_setting = 150;
	float factor_backwards = -1;
	cout << "YEEBUGING: Ben bij dps_setting " << endl;

	while (Color1.reflected_red > average_black_line || Color2.reflected_red > average_black_line){
//		cout << "IN THE WHILE-LOOP" << endl;
		BP.get_sensor(PORT_1, Color1);
		BP.get_sensor(PORT_3, Color2);
	    if(Color1.reflected_red < average_black_line && Color2.reflected_red > average_black_line){
	        BP.set_motor_dps(motor_left, (factor_backwards*dps_setting));
	        BP.set_motor_dps(motor_right,dps_setting);
	    } else if(Color1.reflected_red > average_black_line && Color2.reflected_red < average_black_line){
            	BP.set_motor_dps(motor_left, dps_setting);
            	BP.set_motor_dps(motor_right,(factor_backwards*dps_setting));
	    }
	}
	// The following function-calls make the robot cross the line and make it turn
	cout << "Out of while-loop" << endl;
	crossLine(BP, 90);
	usleep(1000*500);
	driveOnSpot('R', BP);
	usleep(1000*500);
	turnUS(1,BP);
	usleep(1000*200);
}

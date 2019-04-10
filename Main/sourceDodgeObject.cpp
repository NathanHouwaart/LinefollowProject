#include "linefollower.h"

using namespace std;

int current_position_us=0;
vector<float> buffer_motor_values = {};

float vectorAvarage(const vector<float> & to_calculate){
	float sum = 0;
	int total = 0;
	for(float i : to_calculate){
		sum += i;
		total++;
	}
	return sum/total;
}

void turnUS (float values_wheels, BrickPi3 & BP){
	//values_wheels een waarde tussen 0 & 2
	int max_degrees_turned = -95;
	int min_degrees_turned = 95;
	int target_degrees_us = 0;
	int degrees_to_turn = 0;
	uint8_t motor_middle = PORT_B; // Setting the motor to communicate
	int buffersize = 0; //Moet 300 zijn
	if(buffer_motor_values.size() < buffersize){
		buffer_motor_values.push_back(values_wheels);
	} else{
		buffer_motor_values.push_back(values_wheels);
		float avarage_of_vector = vectorAvarage(buffer_motor_values);
		buffer_motor_values = {};
		if(avarage_of_vector == 1){
			target_degrees_us = 0;
		} else if(avarage_of_vector < 1){
			target_degrees_us = min_degrees_turned * (1-avarage_of_vector);
		} else{
			target_degrees_us = (max_degrees_turned * (avarage_of_vector-1));
		}

		degrees_to_turn = target_degrees_us - current_position_us;

//		cout << "avarage_of_vector: " << avarage_of_vector << endl;
//		cout << "current_position " << current_position_us << endl;
//		cout << "target_degrees_us " << target_degrees_us << endl;
//		cout << "Degrees_to_turn " << degrees_to_turn  << endl;
		current_position_us = target_degrees_us;
		//BP.set_motor_limits(motor_middle, 100, 90); // Limit the motor so the gears can keep up
		BP.set_motor_position_relative(motor_middle, degrees_to_turn);
	}
}

void steeringRobot(char to_steer, BrickPi3 & BP){
	/*
	 * This function is called when the robot is driving around the object
	 *
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

void loopForObjectDodge(sensor_ultrasonic_t & UltraSonic, int target_distance , BrickPi3 & BP){
	/* This function is called when driving around the object
	 * It tries to keep the robot/ Ultrasonic sensor add a given distance (target_distance)
	 * This distance is variabele. The current_distance
	 * The loop will stop if the current distance of the
	 */
	getUltraSValue(PORT_4, UltraSonic, BP);
	int current_distance = UltraSonic.cm;
	int factor_difference = 3;

	while(current_distance <= target_distance*factor_difference){
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
		cout << "Current distance: " << current_distance << endl;
	}
	steeringRobot('S', BP);
}

void timeForFlow(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP){
	uint8_t motor_left = PORT_A;
	uint8_t motor_right = PORT_D;
	/*
	BP.set_motor_limits(motor_left,60,200);
	BP.set_motor_limits(motor_right,60,200);

	cout << "Turn on spot" << endl;
	driveOnSpot('R',BP); // Turn robot to the right, on his axis.
	usleep(1000*1500);
	cout << "turn us head" << endl;
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
	cout << "target_distance: " << target_distance << endl;
	//Go straight until one of the color sensors detects a black line.
	while(Color1.reflected_red > average_black_line && Color2.reflected_red > average_black_line){
		cout << "current_distance: " << current_distance << endl;
		if(current_distance > 200){
			current_distance = 100;
			cout << "Ik ben in de if: " << current_distance << endl;
		}
		cout << "target_distance: " << target_distance << endl;

		if(current_distance > target_distance*2){
			steeringRobot('F', BP);
		} else if(current_distance < target_distance){
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
    */
	float dps_setting = 100;

	while (Color1.reflected_red < average_black_line || Color2.reflected_red < average_black_line){
	    if(Color1.reflected_red < average_black_line && Color2.reflected_red > average_black_line){
	        BP.set_motor_dps(motor_left, (-0.1*dps_setting));
	        BP.set_motor_dps(motor_right,dps_setting);
	    } else if(Color1.reflected_red > average_black_line && Color2.reflected_red < average_black_line){
            BP.set_motor_dps(motor_left, dps_setting);
            BP.set_motor_dps(motor_right,(-0.1*dps_setting));
	    }
	}
    /*
	cout << "Out of while-loop" << endl;
	crossLine(BP, 90);
	usleep(1000*500);
	driveOnSpot('R', BP);
	usleep(1000*500);
	turnUS(1,BP);
	usleep(1000*200);
     */
}
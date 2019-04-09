#include "linefollower.h"

using namespace std;

int current_position_us=0;
vector<float> buffer_motor_values = {};


// TODO: --> Complete remake
// Robot rijdt linksom het object
// TODO: --> hoofd draaien & rijden
// TODO: --> Als die over een lijn rijdt en een bocht maakt, hoofd kijkt mee
// TODO: --> Bepalen hoe te sturen bij zien voorwerp
// TODO: --> Lijn terugvinden na object & niet tegen het object aan rijden.

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

	if(buffer_motor_values.size() < 300){
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

	if(to_steer == 'F'){
		// Robot goes straight
		BP.set_motor_position_relative(motor_left, motor_rotation);
		BP.set_motor_position_relative(motor_right, motor_rotation);
	} else if(to_steer == 'L'){
		// Robot steers to left
		BP.set_motor_position_relative(motor_left, (motor_rotation*steering_factor));
		BP.set_motor_position_relative(motor_right, (motor_rotation*(2-steering_factor)));
	} else if(to_steer == 'R'){
		// Robot steers to right
		BP.set_motor_position_relative(motor_left, (motor_rotation*(2-steering_factor)));
		BP.set_motor_position_relative(motor_right, (motor_rotation*steering_factor));
	} else{
		cout << "Wollah dat vind ik niet leuk" << endl;
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
		usleep(1000*600);
		getUltraSValue(PORT_4, UltraSonic, BP);
		current_distance = UltraSonic.cm;
	}
}

void driveAroundObject(sensor_ultrasonic_t & UltraSonic, sensor_color_t & Color1, sensor_color_t & Color2, int average_black_line, BrickPi3 & BP){
	/* This function is called in the Linefollowloop. It is called when te distance between the robot and the object is around 3 cm.
	 * The functions expects all the structs of the 3 different sensors, the target value for the color sensor, the struct of the Pi
	 *
	 */

	uint8_t motor_left = PORT_A;
	uint8_t motor_right = PORT_D;
	driveOnSpot('R',BP); // Turn robot to the right, on his axis.
	turnUS(0,BP); // Turns the US sensor, it makes sure it is turned 90 right-angled on the driving direction
	getUltraSValue(PORT_4, UltraSonic, BP); // reads out the latest data from the sensor

	// Set target distance and start loop
	int target_distance_short = UltraSonic.cm;
	loopForObjectDodge(UltraSonic, target_distance_short, BP);
	// Skkrrrttt a little to the voren
	int32_t short_skrt = 200;
	BP.set_motor_position_relative(motor_left, short_skrt);
	BP.set_motor_position_relative(motor_right, short_skrt);
	usleep(1000*(2*short_skrt));
	// Turn the robot 90 degrees to the left
	BP.set_motor_position_relative(motor_right, 480);
	usleep(1000*(2*480));
	// Drive furhter around the object
	loopForObjectDodge(UltraSonic, target_distance_short, BP);
	// skkrrrttt a robot length to the voren
	int32_t long_skrt = 580;
	BP.set_motor_position_relative(motor_left, long_skrt);
	BP.set_motor_position_relative(motor_right, long_skrt);
	usleep(1000*(2*long_skrt));
	// Turn robot 90 degrees on spot
	driveOnSpot('L', BP);
	// Drive half a length to the voren
	int32_t half_skrt = 290;
	BP.set_motor_position_relative(motor_left, half_skrt);
	BP.set_motor_position_relative(motor_right, half_skrt);
	usleep(1000*(2*half_skrt));

	// Set new target distance and again start loop
	getUltraSValue(PORT_4, UltraSonic, BP);
	int target_distance = UltraSonic.cm;
	int current_distance = UltraSonic.cm;
	BP.get_sensor(PORT_1, Color1);
	BP.get_sensor(PORT_3, Color2);

	//Go straight until one of the color sensors detects a black line.
	while(Color1.reflected_red > average_black_line || Color2.reflected_red > average_black_line){
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
		usleep(1000*600);
		getUltraSValue(PORT_4, UltraSonic, BP);
		current_distance = UltraSonic.cm;
		BP.get_sensor(PORT_1, Color1);
		BP.get_sensor(PORT_3, Color2);
	}
	// The if makes sure both sensors are on the black line
	if(Color1.reflected_red < average_black_line){
		while(Color2.reflected_red > average_black_line){
			BP.set_motor_position_relative(motor_right, 10);
			usleep(1000*(2*10));
		}
	} else{
		while(Color1.reflected_red > average_black_line){
			BP.set_motor_position_relative(motor_right, 10);
			usleep(1000*(2*10));
		}
	}
	// Line detected so turn to right to go on line again
	driveRight(BP);
	turnUS(1, BP);
}


/*
 * 1: Robot ziet voorwerp
 * 2: Robot rijdt door tot ongeveer 3 cm voor het voorwerp.
 * 3: Draait 90 graden op as naar rechts
 * 4: Kijkt naar links
 * 5: Meet afstand --> target_distance
 * 6: while(
 */

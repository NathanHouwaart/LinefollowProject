//
// Created by kevin on 29/03/19.
//

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

void objectDodge(sensor_ultrasonic_t & UltraSonic1, BrickPi3 & BP){
	int afstand_voorwerp_in_cm = 30;
	getUltraSValue(PORT_4, UltraSonic1, BP);
	int check_90_degrees = 0;
	int32_t wheel_left = 2, wheel_right = 1, ultra_sensor_motor = -2;
	while(UltraSonic1.cm < afstand_voorwerp_in_cm){
		BP.set_motor_position_relative(PORT_A, wheel_left);
		BP.set_motor_position_relative(PORT_D, wheel_right);
		BP.set_motor_position_relative(PORT_B, ultra_sensor_motor);
		getUltraSValue(PORT_4, UltraSonic1, BP);
		check_90_degrees += wheel_left;
		if(check_90_degrees >= 500){
			wheel_left = 2, wheel_right = 2, ultra_sensor_motor = 0;
		}
		cout << "Actuele meting: " << UltraSonic1.cm << endl;
	}
	cout << "Actuele meting: " << UltraSonic1.cm << endl;
	for(int i = 0; i < 2; i++) {
		wheel_left = 45, wheel_right = 90;
		BP.set_motor_position_relative(PORT_A, wheel_left);
		BP.set_motor_position_relative(PORT_D, wheel_right);
		getUltraSValue(PORT_4, UltraSonic1, BP);
		int actuele_stand = UltraSonic1.cm;
		wheel_left = 1, wheel_right = 1;
		while (UltraSonic1.cm < actuele_stand) {
			BP.set_motor_position_relative(PORT_A, wheel_left);
			BP.set_motor_position_relative(PORT_D, wheel_right);
			getUltraSValue(PORT_4, UltraSonic1, BP);
		}
	}

}

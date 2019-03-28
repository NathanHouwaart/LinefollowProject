#include "linefollower.h"

using namespace std;

void readMotorEncoders(BrickPi3 & BP){
	/*This function can be used to read the encoders of the motors
	 * Add this time the function is not being used, so I decided not to comment it, because is obselete.
	*/
	string user_input_start;
	string user_input_stop;
	uint8_t motor_right = PORT_D;
	uint8_t motor_left = PORT_A;
	cout << "Type 'start' to start the measuring!" << endl;
	getline(cin, user_input_start);
	if(user_input_start == "start"){
		BP.reset_motor_encoder(motor_right);
		BP.reset_motor_encoder(motor_left);
		cout << "Type 'stop' to stop the measuring!" << endl;
		getline(cin, user_input_stop);
		if(user_input_stop == "stop"){
			int32_t degrees_turned_right, degrees_turned_left;
			BP.get_motor_encoder(motor_right,degrees_turned_right);
			BP.get_motor_encoder(motor_left,degrees_turned_left);
			cout << "left turned: " << degrees_turned_left << " degrees, right turned: " << degrees_turned_right << " degrees." << endl;
		} else{
			cout << "Dat is niet stop!" << endl;
		}
	} else{
		cout << "Dat is geen start! Joe Joe!" << endl;
	}
}

void driveOnSpot(char turn_direction, BrickPi3 & BP){
	/* This function is used to turn 90 degrees on the spot.
	 * The function expects you give the turn side.
	*/
	uint8_t motor_right = PORT_D;
	uint8_t motor_left = PORT_A;
	int32_t turn_90_degrees = 240;
	if(turn_direction != 'L' && turn_direction != 'R'){
		cout << "Nee ik wil een L of een R!" << endl;
	} else if(turn_direction == 'L'){
		BP.set_motor_position_relative(motor_left,(-1*turn_90_degrees));
		BP.set_motor_position_relative(motor_right,turn_90_degrees);
	} else if(turn_direction == 'R'){
		BP.set_motor_position_relative(motor_left,turn_90_degrees);
		BP.set_motor_position_relative(motor_right,(-1*turn_90_degrees));
	}
	usleep(1000*800);
}

#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct, float & target_power, float & kp, float & kd, float & ki, int & lastError, int & integral, int & offset, float turn_modifier) {

    sleep(2);
    while (true) {
        clock_t start = clock();

        BP.get_sensor(PORT_1, Color1);                      // Get sensor data
        int light_value = Color1.reflected_red;             // Get sensor data
//	cin >> light_value;
	// cout << "LIGHT VALUE: " << light_value << endl;

        int error = light_value - offset;                   // Calculate error
        int derivative = error - lastError;
        integral += error;
        int turn = kp * error + kd * derivative + ki * integral;    // Convert error value to turn value

        // cout << "INTEGRAL: " << integral << endl;
        // cout << "TURN: " << turn << endl;

        MotorControllerTurn(turn, target_power, turn_modifier, BP);
        lastError = error;
        printf("Time elapsed: %f\n", ((double) clock() - start) / CLOCKS_PER_SEC);
//        sleep(1);
        // cout << endl;
    }
}

/*  float kpv = (0 - target_power) / (0 - data_struct.difference_min_avarage);
 *  float kdv = 10 * kpv;
 *
 * kpv = (0 - target_power) / (0 - data_struct.difference_min_avarage);
            cout << "New kp value: " << kpv << endl;
            kdv = 10 * kpv;*/

void stopMotor(BrickPi3 & BP) {
    uint8_t motor_right = PORT_D;                       // Here we define the motor ports so we can talk to them
    uint8_t motor_left = PORT_A;
    BP.set_motor_dps(motor_right, 0);               // This stops the motors
    BP.set_motor_dps(motor_left, 0);
}
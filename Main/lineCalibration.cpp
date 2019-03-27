//
// Created by Nathan Houwaart on 2019-03-27.
//

#include "linefollower.h"

using namespace std;

vector<int> calibration(BrickPi3 & BP, sensor_color_t & Color1) {
    /*This function looks for the highest and the lowest light value. The vector contains vector<int> anwser = {low, high};*/

    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    vector<int> low_high_values = {};            // An vector for the return value

    int low;                       // Varialbe for the highest and lowest function
    int high;

    uint16_t speed = 90;               // Om de snelheid van het draaien aan te geven. LET OP niet hoger dan 180!

    sleep(1);

    BP.get_sensor(PORT_1, Color1);		// Get a value
    low = Color1.reflected_red;
    high = Color1.reflected_red;

    BP.set_motor_dps(motor_left, speed);             //the robot turns right
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low){
            low = Color1.reflected_red;
        }
    }

    BP.set_motor_dps(motor_left, (-1 * speed)*2);            // The robot turns motor_left
    BP.set_motor_dps(motor_right, speed*2);
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low) {
            low = Color1.reflected_red;
        }
    }
    BP.set_motor_dps(motor_left, speed);             //the robot turns right backt to its original position
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low){
            low = Color1.reflected_red;
        }
    }
    BP.set_motor_dps(motor_left, 0);
    BP.set_motor_dps(motor_right, 0);

    low_high_values.push_back(low-50);          // Puts the highest value and the lowst value in the vector answer
    low_high_values.push_back(high+20);
    cout << "Lowest measurement " << low << endl;
    cout << "Highest measurement " << high << endl;
    return low_high_values;                  // returns the vector<int> answer
}

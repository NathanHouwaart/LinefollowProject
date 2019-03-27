//
// Created by Nathan Houwaart on 2019-03-27.
//

#include "linefollower.h"

using namespace std;

vector<int> calibartion(BrickPi3 & BP, sensor_color_t & Color1) {
    /*This function looks for the highest and the lowest light value. The vector contains vector<int> anwser = {low, high};*/
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    vector<int> answer = {};            // An vector for the return value
    vector<int> measurments = {};       // An vector for the measurments
    int high = 0;                       // Varialbe for the highest and lowest function
    int low = 1024;
    uint16_t speed = 90;               // Om de snelheid van het draaien aan te geven. LET OP niet hoger dan 180!
    sleep(1);
    BP.get_sensor(PORT_1, Color1);		// Get an value
    measurments.push_back(Color1.reflected_red);      // Put the value in the vector
    BP.set_motor_dps(motor_left, speed);             //the robot turns right
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        measurments.push_back(Color1.reflected_red);           // While the robots turns right the sensor picks up as many measurments as possible. That is why we push it in a vector instead of doing the calculaton immidelay
    }
    BP.set_motor_dps(motor_left, (-1 * speed)*2);            // The robot turns motor_left
    BP.set_motor_dps(motor_right, speed*2);
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        measurments.push_back(Color1.reflected_red);           // Again while ht robot turns the sensor picks up as many measurments as possible.
    }
    BP.set_motor_dps(motor_left, speed);             //the robot turns right backt to its original position
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measuments=0; number_of_measuments<5000;number_of_measuments++) {
        BP.get_sensor(PORT_1, Color1);
        measurments.push_back(Color1.reflected_red);           // Again while ht robot turns the sensor picks up as many measurments as possible.
    }
    BP.set_motor_dps(motor_left, 0);
    BP.set_motor_dps(motor_right, 0);
    for (unsigned int number_of_measuments : measurments) {    // In this for loop he calcualtes the higest value and the lowest value
        if(number_of_measuments == 0){
            low = number_of_measuments;
            high = number_of_measuments;
        }
        if (number_of_measuments < low) low = number_of_measuments;
        if (number_of_measuments > high) high = number_of_measuments;
    }
    answer.push_back(low);          // Puts the highest value and the lowst value in the vector answer
    answer.push_back(high);
    return answer;                  // returns the vector<int> answer
}
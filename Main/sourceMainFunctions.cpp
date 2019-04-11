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

void calibration(sensor_color_t & Color1, CalculatingErrorData & data_struct,  BrickPi3 & BP) {
    /* This function looks for the highest and the lowest light value.
     * The vector contains vector<int> answer = {low, high};
     */
    uint8_t motor_right = PORT_D;                       // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    int low;                                            // Variable for the highest and lowest function
    int high;
    uint16_t speed = 90;                                // Om de snelheid van het draaien aan te geven. LET OP niet hoger dan 180!
    sleep(1);
    BP.get_sensor(PORT_1, Color1);		                 // Get a value
    low = Color1.reflected_red;
    high = Color1.reflected_red;

    BP.set_motor_dps(motor_left, speed);                //the robot turns right
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measurements=0; number_of_measurements<5000;number_of_measurements++) {
        /*Measure 5000 values and save the lowest and highest value*/
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low){
            low = Color1.reflected_red;
        }
    }

    BP.set_motor_dps(motor_left, (-1 * speed)*2);       // The robot turns motor_left
    BP.set_motor_dps(motor_right, speed*2);
    for (unsigned int number_of_measurements=0; number_of_measurements<5000;number_of_measurements++) {
        /*Measure 5000 values and save the lowest and highest value*/
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low) {
            low = Color1.reflected_red;
        }
    }

    BP.set_motor_dps(motor_left, speed);                //the robot turns right backt to its original position
    BP.set_motor_dps(motor_right, (-1 * speed));
    for (unsigned int number_of_measurements=0; number_of_measurements<5000;number_of_measurements++) {
        /*Measure 5000 values and save the lowest and highest value*/
        BP.get_sensor(PORT_1, Color1);
        if(Color1.reflected_red > high){
            high = Color1.reflected_red;
        }else if(Color1.reflected_red < low){
            low = Color1.reflected_red;
        }
    }

    BP.set_motor_dps(motor_left, 0);
    BP.set_motor_dps(motor_right, 0);

    cout << "Lowest measurement " << low << endl;
    cout << "Highest measurement " << high << endl;
    data_struct.lowest_measurment = low;                // Puts the highest value and the lowest value in the struct
    data_struct.highest_measurment = high;
}

bool checkVoltage(BrickPi3 & BP) {
    /* This function looks for the battery voltage to make sure the battery is not empty and we do not damage it */
    if (BP.get_voltage_battery() < 11.0) {              // Checks if battery is empty
        cout << "Battery empty, charge" << endl;
        return false;
    } else {
        cout << BP.get_voltage_battery() << endl;
        cout << BP.get_voltage_9v() << endl;            // If the battery is not empty the battery voltages will displayed in the console
        cout << BP.get_voltage_5v() << endl;
        cout << BP.get_voltage_3v3() << endl;
        return true;
    }
}

void defineDifferenceToAverage(CalculatingErrorData & data_struct) {
    /* In order to calculate how far off the current measurement is from our target, we first need to define the target value.
     * This is done by calculating the average of the min and max reflection value.
     * Then the difference from minimum to avg & maximum to avg is calculated.
     * These values are used to determine how far the current measurement is off from our target on a scale of 0 - 100 %.
     */

    vector<int> calculated_distance_to_average;                                             // Constructs a vector to return multiple values
    int average = (data_struct.lowest_measurment + data_struct.highest_measurment) / 2;     // Calculate average
    int distance_lowest_value_to_avg = average - data_struct.lowest_measurment;             // Calculate distance min--avg
    int distance_highest_value_to_avg = data_struct.highest_measurment - average;           // Calculate distance max--avg

    data_struct.avarage_min_max = average;                                                  //Puts data in struct.
    data_struct.difference_min_avarage = distance_lowest_value_to_avg;
    data_struct.difference_max_avarage = distance_highest_value_to_avg;
}
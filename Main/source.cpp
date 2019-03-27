//
// Created by Nathan Houwaart on 2019-03-25.
//
#include "linefollower.h"

using namespace std;

void limiter(int & right, int & left, const int & maximum_speed = 360) {
    /* This is the limiter function.
     * This function makes sure that the motors don't go above the maximum_speed.
     * It checks if the given motorspeed is above the max, and if it calculates the difference.
     * And retracts this of both motors.
     * We decided to use 2 ifs because then we are sure that none of the motors are going faster.
     */
    if (right > maximum_speed) {
        int verschil_rechts = right - maximum_speed; //Calculates difference
        right -= verschil_rechts; //subtracs the difference from both motors.
        left -= verschil_rechts;
        if(left < 0){ //makes sure that the other motor doesn't go 0.
            left = 0;
        }
    }
    if (left > maximum_speed) {
        int verschil_links = left - maximum_speed; //The same principles apply here as seen above.
        right -= verschil_links;
        left -= verschil_links;
        if(right < 0){
            right = 0;
        }
    }
}

void MotorController(int left, int right, BrickPi3 & BP) {
    /* This function controls the motors.
     * First it sends the given motor speed setting through the limiter
     * After that is sets the motor on the given speed.
     */
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    limiter(right, left,400);                       // The values of the right and left motor are send to the limiter
    BP.set_motor_dps(motor_right, right);       // This starts the motors
    BP.set_motor_dps(motor_left, left);
}

void drive(float direction_control, unsigned int speed_calculator, unsigned int base_speed, BrickPi3 & BP){
    /*
     * direction control is either a value between 0 and 2 or -1 or -2.
     * If the direction control is 1 than the robot will go straight on.
     * if direction control = -1, the robot stops, if direction control = -2, the robot reverses.
     * If direction < 1 and > 0 steer left. > 1 < 2 steer right.
     * speed_calculator is a value between 0 and 100 that is used to set the motor_speed
     * The max_motor_speed is the base value of the motors after calculating with speed_calculator.
     * The base_speed is the value that sets the max speed.
     */
    if(direction_control < -2 || direction_control > 2){
        cout << "Please give a direction control between -2 and 2." << endl;
    } else if(speed_calculator < 0 || speed_calculator > 100){
        cout << "Please give a speed_calculator between 0 and 100." << endl;
    } else{
        int motor_speed = base_speed*(speed_calculator/100.0); // The base motor speed
        int motor_speed_L; //This variable will save the motor speed
        int motor_speed_R;
        if (direction_control >= 0){// && direction_control <= 2){ ---kan ik als het goed is weglaten!---
            motor_speed_L = motor_speed * direction_control;
            motor_speed_R = motor_speed * (2-direction_control);
        } else if (direction_control == -1) {
            motor_speed_L = 0;
            motor_speed_R = 0;
        } else if (direction_control == -2) {
            motor_speed_L = -1 * motor_speed;
            motor_speed_R = -1 * motor_speed;
        } else{
            cout << "The given value doesn't correspondend to the given parameters of x=-2,-1 or 0=<x<=2." << endl;
        }
        //cout << "speed_calculator: " << speed_calculator << " motor_speed: " << motor_speed << " motor L:" << motor_speed_L << " motor R: " << motor_speed_R << endl;
        MotorController(motor_speed_L, motor_speed_R, BP);
    }
}

bool checkVoltage(BrickPi3 & BP) {
    /*This fuction looks for the battery volage to make sure the battery is not empty and we do not damage it*/
    if (BP.get_voltage_battery() < 11.0) {              // If the batery voltage is lower than 11 volt than the battery is empty
        cout << "Battery empty, charge" << endl;
        drive(1,75,360,BP);
        sleep(1);
        drive(11,75,360,BP);                                  // If the battery is empty the robot does a turn right an left and exit the program
        sleep(1);
        drive(0,75,360,BP);
        return false;
    } else {
        cout << BP.get_voltage_battery() << endl;
        cout << BP.get_voltage_9v() << endl;            // If the battery is not empty the battery voltages will displayd in the console
        cout << BP.get_voltage_5v() << endl;
        cout << BP.get_voltage_3v3() << endl;
        return true;
    }
}

bool lightsensorBlackLineDetect(sensor_light_t  & Light1, BrickPi3 & BP) {
    if (BP.get_sensor(PORT_3, Light1) == 0) {
        return (Light1.reflected >= 2400);
    }
}

bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP) {
    if (BP.get_sensor(PORT_1, Color1) == 0) {
        int color_rgb_sensor = Color1.color;
        switch (color_rgb_sensor) {
            case 1:
                return true;
            case 2:
                return false;
            case 3:
                return false;
            case 4:
                return false;
            case 5:
                return false;
            case 6:
                return false;
        }
    }
    return false;
}

void stuur(int lijn_waarde, BrickPi3 & BP) {
    /*In this function we calculate the speed and the direction for the light sensor*/
    int fine_tune_value = 0;
    unsigned int power = 100;                   // The default value of the variable power is 100
    float direction = 0.01 * lijn_waarde + 1;   // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                        // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
        if (power > 100) power = 100;
    }
    if (direction < 1) {                        // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
        if (power > 100) power = 100;
    }
    if (direction == 1) {                       // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    power =fine_tune_value+(power-fine_tune_value)*(limit-fine_tune_value)/(100-fine_tune_value);
    // If the power/ speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    if (power < 10)
        power = 10;
    power = 100-power;
    drive(direction,power,360,BP); // We give the direction and the speed to the function drive
}

vector<int> defineDifference(const int & hoogste, const int & laagste) {
    vector<int> returnvector;
    int gemiddelde = (hoogste + laagste) / 2;
    int laagste_verschil = gemiddelde - laagste;
    int hoogste_verschil = hoogste - gemiddelde;

    returnvector.push_back(gemiddelde);
    returnvector.push_back(laagste_verschil);
    returnvector.push_back(hoogste_verschil);

    return returnvector;
}

int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting) {
    float stuurwaarde, actueel_verschil;
    // optioneel --> tussentijdse  hoogste & laagste waardes veranderen. Dit met bepaalAfwijking

    if (actuele_licht_meting > gemiddelde) {
        actueel_verschil = actuele_licht_meting - gemiddelde;
        stuurwaarde = (actueel_verschil / hoogste_verschil) * 100;
    } else if (actuele_licht_meting < gemiddelde) {
        actueel_verschil = gemiddelde - actuele_licht_meting;
        stuurwaarde = (actueel_verschil / laagste_verschil) * -100;
    } else {
        stuurwaarde = 0;
    }
    return stuurwaarde;
}

vector<int> calibartion(BrickPi3 & BP, sensor_color_t & Color1) {
    /*This function looks for the highest and the lowest light value. The vector contains vector<int> anwser = {low, high};*/
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    vector<int> answer = {};            // An vector for the return value
    vector<int> measurments = {};       // An vector for the measurments
    int high = 0;                       // Varialbe for the highest and lowest function
    int low = 1024;
    uint16_t speed = 90;               // Om de snelheid van het draaien aan te geven. LET OP niet hoger dan 180!
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

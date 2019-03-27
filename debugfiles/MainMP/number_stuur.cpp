//
// Wietse ten Dam at 26-03-2019
//
#include <iostream>
#include <BrickPi3.h>
#include <unistd.h>
#include <vector>
using namespace std;
BrickPi3 BP;

void limiter(int & right, int & left, const int & maximum_speed = 360) {
    /* Dit is de limiter funcite.
    This function makes sure that the motor does not get an value above the maximum_speed and also that the turn speed does not get compromised if one of the motor speeds is reduced */
    if (right > maximum_speed) {
        int verschil_rechts = right - 360;
        right -= verschil_rechts;
        left -= verschil_rechts;
    }
    if (left > maximum_speed) {
        int verschil_links = left - 360;
        right -= verschil_links;
        left -= verschil_links;
    }
}

void MotorController(int left, int right, BrickPi3 & BP) {
    /*This funciton we control the motors*/
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    limiter(right, left);                       // The value of the right and left motor are send to them limiter funcion to exlcude false value's.
    BP.set_motor_dps(motor_right, right);       // This starts the motors at the right speed
    BP.set_motor_dps(motor_left, left);
}

void drive(float value, float power){
    //Value is een waarde tussen 0 en 2 waarbij 1 recht vooruit is. Power is van 1 tot 100
    cout << "De gegeven value is: " << value << endl;
    int motor_power = 360 * (power/100);
    cout << "motorlimiter = " << motor_power << endl;
    int motor_L;
    int motor_R;
    if (value == 1){
        motor_L = motor_power;
        motor_R = motor_power;
    } else if (value >= 0 && value < 1){
        motor_L = motor_power * value;
        motor_R = motor_power * (2-value);
    } else if (value > 1 && value <= 2){
        motor_L = motor_power * (2-value);
        motor_R = motor_power * value;
    } else if (value == -1) {
        motor_L = 0;
        motor_R = 0;
    } else if (value == -2) {
        motor_L = -1 * motor_power;
        motor_R = -1 * motor_power;
    }
    MotorContoller(motor_L, motor_R);
}

bool checkVoltage(BrickPi3 & BP) {
    /*This fuction looks for the battery volage to make sure the battery is not empty and we do not damage it*/
    if (BP.get_voltage_battery() < 11.0) {              // If the batery voltage is lower than 11 volt than the battery is empty
        cout << "Battery empty, charge" << endl;
        drive(1, 75);
        sleep(1);
        drive(11, 75);                                  // If the battery is empty the robot does a turn right an left and exit the program
        sleep(1);
        drive(0, 75);
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
//        cout << "Light sensor (S3): reflected " << setw(4) << light1.reflected << endl;
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

void stuur(float lijn_waarde) {
    /*In this function we calculate the speed and the direction for the light sensor*/
    float power = 100;                                  // The default value of the varable power is 100
    float direction = 0.01 * lijn_waarde + 1;           // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                                // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
        if (power > 100) power = 100;
    }
    if (direction < 1) {                                // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
        if (power > 100) power = 100;
    }
    if (direction == 1) {                               // If the direction is 1 and the robot goes straight the power is 100 
        power = 100;
    }
    float limit = 50;                                   
    power = 0 + (power - 0) * (limit - 0) / (100 - 0);  // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    if (power < 10) power = 10;                         // If the speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    drive(direction, power);                            // We give the directiont and the speed to the function drive
}

vector<int> calibartion() {
    /*This function looks for the highest and the lowest light value. The vector contains vector<int> anwser = {low, high};*/
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    vector<int> answer = {};            // An vector for the return value
    vector<int> measurments = {};       // An vector for the measurments
    int high = 0;                       // Varialbe for the highest and lowest function
    int low = 1024;
    uint32_t range = 8;
    measurments.push_back(BP.get_sensor(PORT_1, Color1));   // We get a base value
    BP.set_motor_position_relative(motor_left, range);             //the robot turns right
    BP.set_motor_position_relative(motor_right, -range);
    for (unsigned int number_of_measuments=0; number_of_measuments<100;number_of_measuments++) {
        measurments.push_back(BP.get_sensor(PORT_1, Color1));           // While the robots turns right the sensor picks up as many measurments as possible.
    }
    BP.set_motor_position_relative(motor_left, -(range * 2));            // The robot turns motor_left
    BP.set_motor_position_relative(motor_right, range * 2);
    for (unsigned int number_of_measuments=0; number_of_measuments<100;number_of_measuments++) {
        measurments.push_back(BP.get_sensor(PORT_1, Color1));           // Again while ht robot turns the sensor picks up as many measurments as possible.
    }
    BP.set_motor_position_relative(motor_left, range);             //the robot turns right backt to its original position
    BP.set_motor_position_relative(motor_right, -range);
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
    return anwser;                  // returns the vector<int> answer
}

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL); // set port 1 to be read as a color sensor
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);   // set port 4 to be read as a light sensor
    sensor_color_t      Color1;                             // Initialise struct for data storage
    sensor_light_t      Light1;                             // Initialise struct for data storage

    if(!checkVoltage(BP)) return 0; // Checks whether battery has enough power

    int current = 1; // Start value for robot. 1 == forward
    while (true) {
        // Hier moet de meer fucntie van het meten van de zwarte lijn en in de funcite moet je de stuur funcite aanroepen of hier in de main funcite
    }
    return 0;
}
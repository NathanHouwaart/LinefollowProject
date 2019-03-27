//
// Created by Nathan Houwaart on 2019-03-25.
//
#include "linefollower.h"

using namespace std;

void limiter(int & right, int & left, const int & maximum_speed = 360) {
    /* Dit is de limiter funcite.
    This function makes sure that the motor does not get an value above the maximum_speed and also that the turn speed does not get compromised if one of the motor speeds is reduced */
    if (right > maximum_speed) {
        int verschil_rechts = right - 360;
        right -= verschil_rechts;
        left -= verschil_rechts;
	if(left < 0){
		left = 0;
	}
    }
    if (left > maximum_speed) {
        int verschil_links = left - 360;
        right -= verschil_links;
        left -= verschil_links;
	if(right < 0){
		right = 0;
	}
    }
}

void MotorController(int left, int right, BrickPi3 & BP) {
    /*This funciton we control the motors*/
    uint8_t motor_right = PORT_D;               // Here are the motor ports defined so we can talk to them
    uint8_t motor_left = PORT_A;
    //cout << "(1)Links: " << left << " Rechts: " << right << endl;
    limiter(right, left);                       // The value of the right and left motor are send to them limiter funcion to exlcude false value's.
    //cout << "(2)Links: " << left << " Rechts: " << right << endl;
    BP.set_motor_dps(motor_right, right);       // This starts the motors at the right speed
    BP.set_motor_dps(motor_left, left);
}

void drive(float value, float power, BrickPi3 & BP){
    //Value is een waarde tussen 0 en 2 waarbij 1 recht vooruit is. Power is van 1 tot 100
    int motor_power = 3*360*(power/100);
    int motor_L;
    int motor_R;
    if (value == 1){
        motor_L = motor_power;
        motor_R = motor_power;
    } else if (value >= 0 && value <= 2){
        motor_L = motor_power * value;
        motor_R = motor_power * (2-value);
    } else if (value == -1) {
        motor_L = 0;
        motor_R = 0;
    } else if (value == -2) {
        motor_L = -1 * motor_power;
        motor_R = -1 * motor_power;
    }
    MotorController(motor_L, motor_R, BP);
}

bool checkVoltage(BrickPi3 & BP) {
    /*This fuction looks for the battery volage to make sure the battery is not empty and we do not damage it*/
    if (BP.get_voltage_battery() < 11.0) {              // If the batery voltage is lower than 11 volt than the battery is empty
        cout << "Battery empty, charge" << endl;
        drive(1, 75, BP);
        sleep(1);
        drive(11, 75, BP);                                  // If the battery is empty the robot does a turn right an left and exit the program
        sleep(1);
        drive(0, 75, BP);
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

void stuur(int lijn_waarde, BrickPi3 & BP) {
//cout << "lijn waarde is: " << lijn_waarde << endl;
    /*In this function we calculate the speed and the direction for the light sensor*/
    float power = 100;                                  // The default value of the varable power is 100
    float direction = 0.01 * lijn_waarde +
                      1;           // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction >
        1) {                                // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
        if (power > 100) power = 100;
    }
    if (direction < 1) {                                // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
        if (power > 100) power = 100;
    }
    if (direction ==
        1) {                               // If the direction is 1 and the robot goes straight the power is 100
        power = 100;
    }
    float limit = 50;
    power = 0 + (power - 0) * (limit - 0) / (100 -
                                             0);  // Here i reduce the scale of the power. Fist we have the range 0 to 100 and than you can change the range to 0 to limit to find the perfect speed
    if (power < 10)
        power = 10;                         // If the speed is lower than 10 the speed will be 10 to prevend the robot form stalling
    drive(direction, power,
          BP);
                            // We give the directiont and the speed to the function drive
}

vector<int> bepaalAfwijking(const int & hoogste, const int & laagste) {
    vector<int> returnvector;
    int gemiddelde = (hoogste + laagste) / 2;
    int laagste_verschil = gemiddelde - laagste;
    int hoogste_verschil = hoogste - gemiddelde;


    returnvector.push_back(gemiddelde);
    returnvector.push_back(laagste_verschil);
    returnvector.push_back(hoogste_verschil);

    return returnvector;
}

int bepaalStuurwaarde(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting) {
    float stuurwaarde, actueel_verschil;
//	cout << "Gemiddelde: " << gemiddelde << " Laagste verschil: " << laagste_verschil << " Hoogste verschil: " << hoogste_verschil << " Actuele meting: " << actuele_licht_meting << endl;
    // optioneel --> tussentijdse  hoogste & laagste waardes veranderen.

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


#include "linefollower.h"

using namespace std;

int defineDirection(const int & gemiddelde, const int & laagste_verschil, const int & hoogste_verschil, const int & actuele_licht_meting) {
    /* Function calculates how far off the current light measurement is off from the target value.
     * Returns a positive percentage if sensor detects more white
     * Returns a negative percentage if sensor detects more black */

    float stuurwaarde, actueel_verschil;
    if (actuele_licht_meting > gemiddelde) {
        actueel_verschil = actuele_licht_meting - gemiddelde;
        stuurwaarde = (actueel_verschil / hoogste_verschil) * 100;
    } else if (actuele_licht_meting < gemiddelde) {
        actueel_verschil = gemiddelde - actuele_licht_meting;
        stuurwaarde = (actueel_verschil / laagste_verschil) * -100;
    } else {
        stuurwaarde = 0;
    }
//    cout << "stuuurwaarde: " << stuurwaarde << endl;
//    cout << "Act verschil: " << actueel_verschil << endl;
//    cout << "hoogste verschil " << hoogste_verschil << endl;
//    cout << "laagste verschil " << laagste_verschil << endl;
    return stuurwaarde;
}

void stuur(int lijn_waarde, BrickPi3 & BP) {
    /* In this function we calculate the speed and the direction for the light sensor*/
    //TODO: wat is lijnwaarde precies?

    int fine_tune_value = 0;
    unsigned int power = 100;                       // The default value of the variable power is 100
//	cout << lijn_waarde << endl;
    float direction = (0.01 * lijn_waarde) + 1;     // Here we calcualte the direction between 0 and 2 for the function drive with the value you of the light sensor. With a P controller form the PID controller
    if (direction > 1) {                            // If the direction is higher than 1 and the robot is going right we calcualte the speed, how shaper he turns how slower it goes
        power = (1 - (direction - 1)) * 100;
        if (power > 100) power = 100;
    }
    else if (direction < 1) {                       // Here does it the same for right but now it goes left
        power = (1 - direction) * 100;
        if (power > 100) power = 100;
    }
    else {                                          // If the direction is 1 and the robot goes straight the power is 100
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

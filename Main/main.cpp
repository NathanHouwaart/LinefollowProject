
//
// Created by Nathan Houwaart on 2019-03-25.
//

#include "linefollower.h"

using namespace std;

void exit_signal_handler(int signo);

BrickPi3 BP;

int main() {

    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED); // set port 1 to be read as a color sensor
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_COLOR_RED); // set port 3 to be read as a color sensor
    sensor_color_t Color1;                             // Initialise struct for data storage color sensor 1
    sensor_color_t Color2;                             // initialise struct for data storage color sensor 2

    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // set port 4 to be read as a ultrasonic sensor
    sensor_ultrasonic_t UltraSonic1;

    // if(!checkVoltage(BP)) return 0; // Checks whether battery has enough power

    vector<int> calibrationvalues = calibration(BP, Color1);
    vector<int> standaardwaardes = defineDifference(calibrationvalues[1], calibrationvalues[0]);
    sleep(2);

    while (true) {
        BP.get_sensor(PORT_1, Color1);
        BP.get_sensor(PORT_3, Color2);
        while(getUltraSValue(PORT_4, UltraSonic1, BP) > 10){
            if(Color2.reflected_red < 300 && Color1.reflected_red < standaardwaardes[0] ){
                cout << "Kruispunt detected" << endl;
                crossroad(BP);
                sleep(0.5);
            }else{
                int stuurwaarde = defineDirection(standaardwaardes[0], standaardwaardes[1], standaardwaardes[2],
                                                  Color1.reflected_red);

                stuur(stuurwaarde, BP);
            }
        }
    }
    return 0;
}

void exit_signal_handler(int signo) {
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

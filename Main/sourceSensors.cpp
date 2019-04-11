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

int getUltraSValue(uint8_t port, sensor_ultrasonic_t & data_struct, BrickPi3 & BP) {
    /*-----Function reads Ultrasonic Sensor (US) and returns measured distance in cm.
     * Since the US sensor is not completely accurate, there will be a correction of -1 cm-----*/
    BP.get_sensor(port, data_struct);                           //Getting sensordata and storing it in a given struct
    return static_cast<int>(data_struct.cm) - 1;
}

//WARNING: THIS FUNCTION IS NOT USED
void objectDetect(sensor_ultrasonic_t &data_struct, BrickPi3 & BP, int threshold) {
    /*-----need to make playing a parameter so it can be used and be the same in multiple functions-----*/
    while (true) {
        int distance = getUltraSValue(PORT_4, data_struct, BP);         //Get US sensor data
        int motorSpeed = 180;
        MotorController(motorSpeed, motorSpeed, BP);
        while (distance < (threshold +10)) {
            if(distance < threshold){
                if(distance > (threshold -1)) {
                    motorSpeed = 0;
                } else {
                    motorSpeed = (threshold - distance) * -18;
                }
            } else {
                motorSpeed = (distance - threshold) * 18;
            }
            MotorController(motorSpeed, motorSpeed, BP);
            distance = getUltraSValue(PORT_4, data_struct, BP);
            usleep(250 * 1000);
        }
    }
}

//WARNING: THIS FUNCTION IS NOT USED
bool colorsensorBlackLineDetect(sensor_color_t & Color1, BrickPi3 & BP) {
    /* Function checks if color sensor on port 3 (Hardcoded) has detected a black line.
     * If color == black it returns true.
     * Otherwise it returns false
     */
    // TODO: --> Check if return false can only be at case 6.

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
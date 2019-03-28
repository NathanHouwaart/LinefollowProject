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
    //    cout << "Afstand:   " << (data_struct.cm - 1) << " cm." << endl;  //Printing the current centimeter value in the struct.
    return static_cast<int>(data_struct.cm) - 1;
}

void objectDetect(sensor_ultrasonic_t & data_struct, BrickPi3 & BP, int threshold) {
    while (true) {
        int distance = getUltraSValue(PORT_4, data_struct, BP);
        MotorController(180, 180, BP);

        while (distance < threshold) {
            MotorController(0, 0, BP);
            distance = getUltraSValue(PORT_4, data_struct, BP);
            usleep(500 * 1000);
        }
    }
}

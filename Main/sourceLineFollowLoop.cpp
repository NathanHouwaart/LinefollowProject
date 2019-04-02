#include "linefollower.h"

using namespace std;
// TODO: --> Mogelijke aanpassing door verandering object ontwijken.
// TODO: --> Meting lager of hoger dan min of max meting (calibratie) Waarde laag en hoog aanpassen.
int counter_object = 0;
void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & Ultrasonic1, vector<int> & min_max_reflection_value, vector<int> & default_values, BrickPi3 & BP){
    while (true) {
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);
        //cout << "Afstand is: " <<  getUltraSValue(PORT_4, UltraSonic1, BP);
        if (getUltraSValue(PORT_4, UltraSonic1, BP) > 10) {       // If the measured US distance is bigger than 10:
            counter_object = 0;
            if (Color2.reflected_red < (default_values[0]) && Color1.reflected_red < default_values[0]) {
                crossroad(BP);
            } else {                                             // If no intersection was detected, follow the line
                int stuurwaarde = defineDirection(default_values[0], default_values[1], default_values[2],
                                                  Color1.reflected_red);
                stuur(stuurwaarde, BP);
            }
        } else {                                                  // If an object was detected within X cm, execute this code
            drive(DIRECTION_STOP, 0, 360, BP); // Stop the car
            counter_object++;
            if (counter_object % 1000 == 0) { cout << "Joe 1000 iets" << counter_object << endl; }
            if (counter_object >= 1500) {
                cout << "Counter is groot genoeg" << endl;
                driveAround(BP);
            }//Start driving around milk
        }
    }
}
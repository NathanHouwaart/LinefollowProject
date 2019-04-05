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

// TODO: --> Mogelijke aanpassing door verandering object ontwijken.
// TODO: --> Meting lager of hoger dan min of max meting (calibratie) Waarde laag en hoog aanpassen.
//TODO: --> Een struct maken waar de min,max,avarage,distance_min,distance_max inzetten?

int counter_object = 0;

void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP){
    SharpCornerSettings StructSharpCorner;

    while (true) {
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);
        int main_sensor_measurment = Color1.reflected_red;
        if(main_sensor_measurment < data_struct.lowest_measurment){
            data_struct.lowest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurment > data_struct.highest_measurment){
            data_struct.highest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 10) {       // If the measured US distance is bigger than 10:
            counter_object = 0;
            if (Color2.reflected_red < data_struct.avarage_min_max && main_sensor_measurment < data_struct.avarage_min_max) {
                crossroad(BP, StructSharpCorner);
            } else {                                             // If no intersection was detected, follow the line
                int error_to_avarage = defineError(data_struct.avarage_min_max, data_struct.difference_min_avarage, data_struct.difference_max_avarage, main_sensor_measurment);
                pController(error_to_avarage, BP, StructSharpCorner);
            }
        } else {                                                  // If an object was detected within X cm, execute this code
            objectDodge(UltraSonic, BP);
        }
    }
}

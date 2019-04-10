#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct) {
    int tp = 360;
    float kp = -1.00;
    while (true) {

        int offset = data_struct.avarage_min_max;

        BP.get_sensor(PORT_1, Color1);
        int light_value = Color1.reflected_red;
        if(light_value < data_struct.lowest_measurment){
            data_struct.lowest_measurment = light_value;
            defineDifferenceToAverage(data_struct);
        } else if(light_value > data_struct.highest_measurment){
            data_struct.highest_measurment = light_value;
            defineDifferenceToAverage(data_struct);
        }

        int error = light_value - offset;
        int turn = kp * error;
        int speedA = tp + turn;
        int speedD = tp - turn;
        MotorController(speedD, speedA, BP);
        cout << speedA << " " << speedD << endl;
    }
}

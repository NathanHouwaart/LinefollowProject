#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP) {
    int tp = 360;
    float kp = -0.847;
    int offset = 425;
    while (true) {
        BP.get_sensor(PORT_1, Color1);
        int light_value = Color1.reflected_red;
        int error = light_value - offset;
        int turn = kp * error;
        int speedA = tp + turn;
        int speedD = tp - turn;
        MotorContoller(speedA, speedD, BP);
        cout << speedA << " " << speedD << endl;
    }
}

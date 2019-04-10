#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct) {
    float tp = 50;                                           // Constant value to determine maximum motor dps
    float kp = 0.25;                                       // Constant value to determine the sharpness of the turns the robot takes
    float kpv = (0 - tp)/(0 - data_struct.difference_min_avarage);
    float kd = 1.3;
    float ki = 100;
    int lastError = 0;

    while (true) {

        int offset = data_struct.avarage_min_max;           // Target ligh value for the robot to follow

        BP.get_sensor(PORT_1, Color1);                      // Get sensor data
        int light_value = Color1.reflected_red;             // Get sensor data
        if(light_value < data_struct.lowest_measurment){    // Checks whether the lowest measured value needs to be updated
            data_struct.lowest_measurment = light_value;
            defineDifferenceToAverage(data_struct);
            kpv = (0 - tp)/(0 - data_struct.difference_min_avarage);
        } else if(light_value > data_struct.highest_measurment){ // Checks whether the hightest measured light value needs to be updated
            data_struct.highest_measurment = light_value;
            defineDifferenceToAverage(data_struct);         // Recalculate data
            kpv = (0 - tp)/(0 - data_struct.difference_min_avarage);
        }

        int error = light_value - offset;                   // Calculate error
        int derivative = error - lastError;
        int integral = integral + error;
        int turn = kp * error + kd * derivative + ki * integral;                              // Convert error value to turn value
        int speedA = tp + turn;                             // Set new speed of motor A
        int speedD = tp - turn;                             // Set new speed of motor B
        MotorControllerPower(speedD, speedA, BP);                // Update motor speed
        lastError = error;
    }
}

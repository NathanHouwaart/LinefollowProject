#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct) {
    float target_power = 30;                                // Constant value to determine maximum motor dps
    float kp = 0.42;                                        // 100/ (((200+680)/2) - 200)
    float kd = 0;                                         // Constant value to determine the sharpness of the turns the robot takes. was 3.0
    float ki = 0;                                           //0.0000001
    int lastError = 0;
    int integral = 0;
    int offset = data_struct.avarage_min_max;           // Target ligh value for the robot to follow
    cout << "OFFSET: " << offset << endl;
    float turn_modifier = -2.0 * target_power / 100;

    while (true) {
        clock_t start = clock();

        BP.get_sensor(PORT_1, Color1);                      // Get sensor data
        int light_value = Color1.reflected_red;             // Get sensor data

        if (light_value < data_struct.lowest_measurment) {    // Checks whether the lowest measured value needs to be updated
            data_struct.lowest_measurment = light_value;
            defineDifferenceToAverage(data_struct);
            offset = data_struct.avarage_min_max;
        } else if (light_value > data_struct.highest_measurment) { // Checks whether the hightest measured light value needs to be updated
            data_struct.highest_measurment = light_value;
            defineDifferenceToAverage(data_struct);         // Recalculate data
            offset = data_struct.avarage_min_max;
        }

        int error = light_value - offset;                   // Calculate error
        int derivative = error - lastError;
        integral += error;
        int turn = kp * error + kd * derivative + ki * integral;    // Convert error value to turn value

        cout << "INTEGRAL: " << integral << endl;
        cout << "TURN: " << turn << endl;

        MotorControllerTurn(turn, target_power, turn_modifier, BP);
        lastError = error;
        printf("Time elapsed: %f\n", ((double) clock() - start) / CLOCKS_PER_SEC);
    }
}

/*  float kpv = (0 - target_power) / (0 - data_struct.difference_min_avarage);
 *  float kdv = 10 * kpv;
 *
 * kpv = (0 - target_power) / (0 - data_struct.difference_min_avarage);
            cout << "New kp value: " << kpv << endl;
            kdv = 10 * kpv;
            cout << kd << endl; */
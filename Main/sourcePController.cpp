#include "linefollower.h"

using namespace std;

void PController(sensor_color_t & Color1, BrickPi3 & BP, CalculatingErrorData & data_struct, float & target_power, float & kp, float & kd, float & ki, int & lastError, int & integral, int & offset, float turn_modifier) {
    /* This function is the complete PID controller.
     * It calculates the turn value for the robot with:
     *      - Proportional error
     *      - Derivative error
     *      - Integral error
     *  This is mulitplied by K values to calculate turn.
     *  K values may be tweaked to make the line follow proces smoother.
     */

    BP.get_sensor(PORT_1, Color1);                                      // Get sensor data
    int light_value = Color1.reflected_red;                             // Get sensor data
    int error = light_value - offset;                                   // Calculate error
    int derivative = error - lastError;                                 // Calculate derivative
    integral += error;                                                  // Calculate error
    int turn = kp * error + kd * derivative + ki * integral;            // Convert error values to turn value

    MotorControllerTurn(turn, target_power, turn_modifier, BP);         // Motors are controlled with the correct turn value
    lastError = error;                                                  // Sets last error as current error for derivative
}

void stopMotor(BrickPi3 & BP) {
    /* Funciton to stop the robot */

    uint8_t motor_right = PORT_D;                   // Here we define the motor ports so we can talk to them
    uint8_t motor_left = PORT_A;
    BP.set_motor_dps(motor_right, 0);               // This stops the motors
    BP.set_motor_dps(motor_left, 0);
}

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "BrickPi3.h"
#inlcude "linefollower.h"
using namespace std;
BrickPi3 BP;

void limiter(int & right, int & left, const int & maximum_speed) {
    if (right > maximum_speed) {
        int verschil_rechts = right - 360;
        right -= verschil_rechts;
        left -= verschil_rechts;
    }
    if (left > maximum_speed) {
        int verschil_links = left - 360;
        right -= verschil_links;
        left -= verschil_links;
    }
}

void Motor_controller(int left, int right) {
    uint8_t motor_right = PORT_D;
    uint8_t motor_left = PORT_A;
    limiter(right, left, 360);
    BP.set_motor_dps(motor_right, right);
    BP.set_motor_dps(motor_left, left);
}

void drive(int direction) {  // direction is 0 tot 11
    if (direction > 6) {     // rechts af
        if (direction == 11) {
            Motor_controller(360, -360);
        }
        if (direction == 10) {
            Motor_controller(324, 36);
        }
        if (direction == 9) {
            Motor_controller(288, 72);
        }
        if (direction == 8) {
            Motor_controller(252, 108);
        }
        if (direction == 7) {
            Motor_controller(216, 144);
        }
    }
    if (direction < 6) {     // links af
        if (direction == 1) {
            Motor_controller(-360, 360);
        }
        if (direction == 2) {
            Motor_controller(36, 324);
        }
        if (direction == 3) {
            Motor_controller(72, 288);
        }
        if (direction == 4) {
            Motor_controller(108, 252);
        }
        if (direction == 5) {
            Motor_controller(144, 216);
        }
    }
    if (direction == 6) {    // rechtdoor
        Motor_controller(180, 180);
    }
    if (direction == 0) {
        Motor_controller(0, 0);
    }
    if (direction == -1) {
        Motor_controller(-180, -180);
    }
}

void exit_signal_handeler(int signo) {
    if (signo == SIGINT) {
        BP.reset_all();
        exit(-2);
    }
}


int main() {
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

    sensor_color_t      Color1;
    sensor_light_t      Light1;

    if (BP.get_voltage_battery() < 11.0) {
        cout << "Battery empty, charge" << endl;
        drive(1);sleep(1);drive(11);sleep(1);drive(0);
        return 0;
    } else {
        cout << BP.get_voltage_battery() << endl;
        cout << BP.get_voltage_9v() << endl;
        cout << BP.get_voltage_5v() << endl;
        cout << BP.get_voltage_3v3() << endl;
    }
    int current = 6;
    while (true) {
        if (lightsensorBlackLineDetect(Light1)) {
            current++;
            drive(current);
        }
        if (colorsensorBlackLineDetect(Color1)) {
            current--;
            drive(current);
        }
        if (lightsensorBlackLineDetect(Light1) && colorsensorBlackLineDetect(Color1)) {
            drive(0);
        }
    }
    return 0;
}


#include <iostream>
#include <signal.h>
#include "BrickPi3.h"
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

void Motor_contoller(int left, int right) {
    uint8_t motor_right = PORT_D;
    uint8_t motor_left = PORT_A;
    limiter(right, left, 360);
    BP.set_motor_dps(motor_right, right);
    BP.set_motor_dps(motor_left, left);
}

void drive(int direction) {  // direction is 0 tot 11
    if (direction > 6) {     // rechts af
        if (direction == 11) {
            Motor_contoller(360, -360);
        }
        if (direction == 10) {
            Motor_contoller(324, 36);
        }
        if (direction == 9) {
            Motor_contoller(288, 72);
        }
        if (direction == 8) {
            Motor_contoller(252, 108);
        }
        if (direction == 7) {
            Motor_contoller(216, 144);
        }
    }
    if (direction < 6) {     // links af
        if (direction == 1) {
            Motor_contoller(-360, 360);
        }
        if (direction == 2) {
            Motor_contoller(36, 324);
        }
        if (direction == 3) {
            Motor_contoller(72, 288);
        }
        if (direction == 4) {
            Motor_contoller(108, 252);
        }
        if (direction == 5) {
            Motor_contoller(144, 216);
        }
    }
    if (direction == 6) {    // rechtdoor
        Motor_contoller(180, 180);
    }
    if (direction == 0) {
        Motor_contoller(0, 0);
    }
    if (direction == -1) {
        Motor_contoller(-180, -180);
    }
}

void exit_signal_handeler(int signo) {
    if (signo == SIGINT) {
        BP.reset_all();
        exit(-2);
    }
}


int main() {
    signal(SIGINT, exit_signal_handeler);
    BP.detect();
    int number;
    cout << "Number between -1 and 11. Enter 999 for reset" << endl;
    while (true) {
        cout << "->";
        cin >> number;
        cout << endl;
        if (number == 999) {
            BP.reset_all();
        } else {
            drive(number);
        }
    }
    return 0;
}


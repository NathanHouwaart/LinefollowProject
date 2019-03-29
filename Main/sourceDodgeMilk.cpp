//
// Created by kevin on 29/03/19.
//

#include "linefollower.h"

using namespace std;

void driveAround(BrickPi3 & BP) {
    driveOnSpot('R', BP);
    MotorController(360, 360, BP);
    sleep(1);
    driveOnSpot('L', BP)
    MotorController(360, 360, BP);
    sleep(2);
    driveOnSpot('R', BP);
    MotorController(360, 360, BP);
    sleep(1);
    MotorController(0, 0, BP);
}
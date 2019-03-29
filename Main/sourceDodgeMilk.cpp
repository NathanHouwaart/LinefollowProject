//
// Created by kevin on 29/03/19.
//

#include "linefollower.h"

using namespace std;

void driveAround(BrickPi3 & BP) {
    driveOnSpot('R', BP);
    MotorController(360, 360, BP);
    usleep(1500*1000);
    driveOnSpot('L', BP);
    MotorController(360, 360, BP);
    usleep(3500*1000);
    driveOnSpot('L', BP);
    MotorController(360, 360, BP);
    usleep(1600*1000);
    driveOnSpot('R', BP);
}

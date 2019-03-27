#include "linefollower.h"

using namespace std;

void driveLeft(BrickPi3 & BP) {
    drive(LEFT, 99, 360, BP); //0 = left
    usleep(1200 * 1000);
    driveForward(BP);
}

void driveRight(BrickPi3 & BP) {
    drive(RIGHT, 99, 360, BP); //2 = right
    usleep(1200 * 1000);
    driveForward(BP);
}

void driveForward(BrickPi3 & BP) {
    drive(FORWARD, 99, 360, BP); //drive forward
    usleep(500 * 1000); // Waiting for half a second, the car can cross the crossroad without seeing the line again
}

void crossroad(BrickPi3 & BP) {
    drive(STOP, 0, 360, BP); //stop the car
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    char choice;
    cin >> choice;
    switch (choice) {
        case 'L':
            driveLeft(BP);
            break;
        case 'R':
            driveRight(BP);
            break;
        case 'F':
            driveForward(BP);
            break;
        default:
            cout << "Wrong input. Please try again";
            crossroad(BP);
    }
}


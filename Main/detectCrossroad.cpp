#include "linefollower.h"

using namespace std;

void driveLeft(BrickPi3 & BP){

}

void driveRight(BrickPi3 & BP){

}

void driveForward(BrickPi3 & BP){
    drive(1,100,360,BP);
    sleep(0.5);
}

void crossroad(BrickPi3 & BP){
    drive(-1, 0, 360, BP);
    cout << "Crossroad detected: Do you want to go LEFT(L)/RIGHT(R)/FORWARD(F)" << endl;
    char choice;
    cin >> choice;
    switch(choice){
        case 'L':
            driveLeft(BP);
            break;
        case 'R':
            driveRight(BP);
            break;
        case 'F':
            driveForward(BP);
        default:
            cout << "Wrong input. Please try again"
            crossroad(BP);
    }
}


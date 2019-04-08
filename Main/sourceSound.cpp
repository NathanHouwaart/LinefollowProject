//
// Created by kevin on 02/04/19.
//
#include "linefollower.h"
using namespace std;

// playing the right sound by asking for a parameter
void playSound(char selection, int & playing) {
    /*
    playing = 0; means there is no sound playing currently
    playing = 1; The forward sound is playing
    playing = 2; The backwards sound is playing
    playing = 3; The turnleft sound is playing
    playing = 4; The turnright sound is playing
    playing = 5; straight across
    playing = 6; the See object sound is playing
    playing = 7; dodge object
    playing = 8; crossroad
    playing = 9; megalovania
    */
    switch(selection) {
        case 'F':  //forward
            if(playing != 1) {
                stopSound();
                system("omxplayer --loop --no-keys -o local Noises/forward.wav &");
                playing = 1;
            }
            break;
        case 'B':  //backwards
            if(playing != 2) {
                stopSound();
                system("omxplayer --loop --no-keys -o local Noises/backwards.wav &");
                playing = 2;
            }
            break;
        case 'L':  //turn left on crossroad
            if(playing != 3) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/turnleft.wav &");
                playing = 3;
            }
            break;
        case 'R':  //turn right on crossroad
            if(playing != 4) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/turnright.wav &");
                playing = 4;
            }
            break;
        case 'S':  //drive straight over a crossroad
            if(playing != 5) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/drivestraight.wav &");
                playing = 5;
            }
            break;
        case 'O':  //see object
            if(playing != 6) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/seeobject.wav &");
                playing = 6;
            }
            break;
        case 'D':  //dodge object
            if(playing != 7) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/dodge.wav &");
                playing = 7;
            }
            break;
        case 'C':  //crossroad
            if(playing != 8) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/crossroad.wav &");
                playing = 8;
            }
            break;
        case 'X':  //plan X, playing megalovania while charging at the object
             if(playing != 9) {
                 stopSound();
                 system("omxplayer --no-keys -o local Noises/megalovaniaRobotCharge.wav &");
                 playing = 9;
             }
             break;
    }
}

//stopping the sound because the time to play can be different
void stopSound() {
    system("killall omxplayer.bin");
}

void megaCharge(int & playing, BrickPi3 & BP) {
    drive(DIRECTION_STOP, 0, 360, BP); //stop the car
    cout << "Object detected: What do you want to do, type: D(dodge) or X(Plan X)" << endl;
    char choice;
    cin >> choice;
    switch (choice) {
        case 'X':
            playSound('X', playing);
            usleep(4500 *1000);
            drive(1, 100, 360, BP);
            sleep(11);
            drive(DIRECTION_STOP, 0, 360, BP);

            break;
        case 'D':
            driveAround(BP);
            break;
        default:
            cout << "Wrong input. Please try again";
            megaCharge(playing, BP);
    }
}

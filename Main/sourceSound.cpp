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
    playing = 3; The turnonspot sound is playing
    playing = 4; the See object is playing
    playing = 5; doge object sound is playing
    playing = 6; the crossroad sound is playing
    playing = 7; beta megalovania
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
        case 'T':  //turn on spot
            if(playing != 3) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/turnonspot.wav &");
                playing = 3;
            }
            break;
        case 'S':  //see object
            if(playing != 4) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/seeobject.wav &");
                playing = 4;
            }
            break;
        case 'D':  //dodge object
            if(playing != 5) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/dodge.wav &");
                playing = 5;
            }
            break;
        case 'C':  //crossroad
            if(playing != 6) {
                stopSound();
                system("omxplayer --no-keys -o local Noises/crossroad.wav &");
                playing = 6;
            }
            break;
        case 'X':  //plan X, playing megalovania while charging at the object
             if(playing != 7) {
                 stopSound();
                 system("omxplayer --no-keys -o local Noises/megalovaniaRobotCharge.wav &");
                 playing = 7;
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

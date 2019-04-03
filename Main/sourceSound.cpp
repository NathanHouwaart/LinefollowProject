//
// Created by kevin on 02/04/19.
//
#include "linefollower.h"
using namespace std;

// playing the right sound by asking for a parameter
void playSound(char & selection) {
    switch(selection) {
        case 'F':  //forward
            system("omxplayer --no-keys -o local Noises/forward.wav &");
            break;
        case 'B':  //backwards
            system("omxplayer --no-keys -o local Noises/backwards.wav &");
            break;
        case 'T':  //turn on spot
            system("omxplayer --no-keys -o local Noises/turnonspot.wav &");
            break;
        case 'S':  //see object
            system("omxplayer --no-keys -o local Noises/seeobject.wav &");
            break;
        case 'D':  //dodge object
            system("omxplayer --no-keys -o local Noises/dodge.wav &");
            break;
        case 'C':  //crossroad
            system("omxplayer --no-keys -o local Noises/crossroad.wav &");
            break;
        /*case 'X':  //plan X, playing megalovania while charging at the object
            system("omxplayer --no-keys -o local Noises/megalovania.wav &");
            break;*/
    }
}

//stopping the sound because the time to play can be different
void stopSound() {
    system("killall omxplayer.bin")
}

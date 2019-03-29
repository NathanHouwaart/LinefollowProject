#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
//#include <softPwm.h>

using namespace std;

#define buzzer_pin 10                // Deze moet vrij zijn van de brickpi, het de de 8 ste pin van boven in de recht kolom



int main(void) {

    if(wiringPiSetup()==-1) {
        cout<<"Setup wiring pi failed";
        return 1;
    }
    pinMode(buzzer_pin,OUTPUT);
//    softPwmCreate(buzzer_pin,50,100);
    bool flip = 0;
    int counter = 0;
    while(1) {
        sleep(1);
        if (flip == 0) {
            counter += 100;
        } else {
            counter += 100;
        }
        pwmWrite(buzzer_pin,counter);
        if (counter == 1023) {
            flip = 1;
        }
        if (counter == 0) {
            flip = 0;
        }
    }
return 0;
}


/*
http://wiringpi.com/pins/
https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/
https://github.com/DexterInd/BrickPi3/blob/master/Hardware/BrickPi%203.2.1.pdf
*/

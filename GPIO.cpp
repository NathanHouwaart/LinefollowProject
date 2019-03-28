#include<iostream>
#include<wiringPi.h>
#include <softPwm.h>

using namespace std;

#define buzzer_pin 4                // Deze moet vrij zijn van de brickpi, het de de 8 ste pin van boven in de recht kolom



int main(void) {

    if(wiringPiSetupGpio()==-1) {
        cout<<"Setup wiring pi failed";
        return 1;
    }
    pinMode(buzzer_pin,OUTPUT);
    softPwmCreate(buzzer_pin,0,100);

    while(1) {
        int pwm_value = 0;
        cout << "-> ";
        cin >> pwm_value;
        softPwmWrite(buzzer_pin,pwm_value);
    }
return 0;
}


/*
http://wiringpi.com/pins/
https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/
https://github.com/DexterInd/BrickPi3/blob/master/Hardware/BrickPi%203.2.1.pdf
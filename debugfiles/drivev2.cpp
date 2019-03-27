#include <iostream>
#include <BrickPi3.h>
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

void Motor_controller(int left, int right) {
    uint8_t motor_right = PORT_D;
    uint8_t motor_left = PORT_A;
    limiter(right, left, 360);
    BP.set_motor_dps(motor_right, right);
    BP.set_motor_dps(motor_left, left);
}

void motordingen(float value, float power){
    //Value is een waarde tussen 0 en 2 waarbij 1 recht vooruit is. Power is van 1 tot 100
    cout << "De gegeven value is: " << value << endl;
    int motorpower = 360 * (power/100);
    cout << "motorlimiter = " << motorpower << endl;
    int motorL;
    int motorR;
    if(value == 1){
        motorL = motorpower;
        motorR = motorpower;
    } else if(value < 1){
        motorL = motorpower * value;
        motorR = motorpower * (2-value);
    } else if(value > 1 && value <= 2){
        motorL = motorpower * (2-value);
        motorR = motorpower * value;
    }
    Motor_contoller(motorL, motorR);
}

int main(){
    motordingen(0.99,100);
    return 0;
}
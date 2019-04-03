#include "linefollower.h"
using namespace std;


bool lookLeft(BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    BP.set_motor_position_relative(motor_middle, -90);
    sleep(2);
    BP.set_motor_position_relative(motor_middle, 90);
}

bool lookRight(BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    BP.set_motor_position_relative(motor_middle, 90);
    sleep(2);
    BP.set_motor_position_relative(motor_middle, -90);
}

bool lookForward(BrickPi3 & BP){

}
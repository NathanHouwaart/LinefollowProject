#include "linefollower.h"
using namespace std;


bool lookLeft(BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    int32_t draai = 130;
    BP.set_motor_limits(PORT_B, 60, 90);
    BP.set_motor_position_relative(motor_middle, draai*-1);
    sleep(2);
    BP.set_motor_position_relative(motor_middle, draai+10);
}

bool lookRight(BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    int32_t draai = 110;
    BP.set_motor_limits(PORT_B, 60, 90);
    BP.set_motor_position_relative(motor_middle, draai);
    sleep(2);
    BP.set_motor_position_relative(motor_middle, draai*-1-10);
}

bool lookForward(BrickPi3 & BP){

}

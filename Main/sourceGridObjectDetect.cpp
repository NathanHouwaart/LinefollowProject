#include "linefollower.h"
using namespace std;


bool lookLeft(sensor_ultrasonic_t UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    int32_t draai = 130;
    BP.set_motor_limits(PORT_B, 60, 90);
    BP.set_motor_position_relative(motor_middle, draai*-1);
    sleep(1);
    cout << "Waarde is: " << getUltraSValue(PORT_4, UltraSonic, BP) << endl;
    sleep(2);
    BP.set_motor_position_relative(motor_middle, draai+10);
}

bool lookRight(sensor_ultrasonic_t UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;
    int32_t draai = 110;
    BP.set_motor_limits(PORT_B, 60, 90);
    BP.set_motor_position_relative(motor_middle, draai);
    sleep(1);
    cout << "Waarde is: " << getUltraSValue(PORT_4, UltraSonic, BP) << endl;
    sleep(1);
    BP.set_motor_position_relative(motor_middle, draai*-1-10);
}

bool lookForward(BrickPi3 & BP){
    cout << "Waarde is: " << getUltraSValue(PORT_4, UltraSonic, BP) << endl;
}

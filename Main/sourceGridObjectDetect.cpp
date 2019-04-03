#include "linefollower.h"
using namespace std;


bool lookLeft(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B; //Setting the motor to communicate
    int32_t draai = 130; // The degrees needed to turn the USsensor with gears
    BP.set_motor_limits(PORT_B, 100, 90); // Limit the motor so the gears can keep up
    BP.set_motor_position_relative(motor_middle, draai*-1); // Turn  the motor -draai- degrees 
    sleep(1);
    lookForward(UltraSonic, BP);
    sleep(1);
    BP.set_motor_position_relative(motor_middle, draai+10); // Turn the motor back to original position
}

bool lookRight(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B; // Setting the motor to communicate
    int32_t draai = 110; // The degrees needed to turn the motor Right
    BP.set_motor_limits(PORT_B, 100, 90); // Limit the motor so the gears can keep up
    BP.set_motor_position_relative(motor_middle, draai);// Turn the motor -draai- degrees Right
    sleep(1);
    lookForward(UltraSonic, BP);
    sleep(1);
    BP.set_motor_position_relative(motor_middle, draai*-1-10);// Turn the motor back to original position
}

bool lookForward(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP){
    getUltraSValue(PORT_4, UltraSonic, BP); //Get value of USsensor and put it in a struct
    float afstand_in_centimeter = UltraSonic.cm; //Get the last struct value
    cout << "Waarde is: " << afstand_in_centimeter << endl; //Print the last struct value (last measurement)
}

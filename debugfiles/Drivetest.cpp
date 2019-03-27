#include "BrickPi3.h"
#include <signal.h>
#include <unistd.h>
BrickPi3 BP;

void exit_signal_handler(int signo);

void forward(uint8_t motor_links, uint8_t motor_rechts, int vooruit) {
    BP.set_motor_dps(motor_rechts, vooruit);
    BP.set_motor_dps(motor_links, vooruit);
}

void backward(uint8_t motor_links, uint8_t motor_rechts, int achteruit) {
    BP.set_motor_dps(motor_rechts, achteruit);
    BP.set_motor_dps(motor_links, achteruit);
}


int main() {
    uint8_t motor_rechts = PORT_D;
    uint8_t motor_links = PORT_A;
    int draai_vooruit = 360;
    int draai_achteruit = -360;

    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    forward(motor_links, motor_rechts, draai_vooruit);
    sleep(1000);
    backward(motor_links,motor_rechts, draai_achteruit);
    sleep(1000);

    return 0;
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}


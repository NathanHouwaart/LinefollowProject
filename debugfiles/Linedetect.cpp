#include "BrickPi3.h" // for BrickPi3
#inlcude "linefollower.h"
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);
bool lightsensorBlackLineDetect(sensor_light_t & Light);
bool colorsensorBlackLineDetect(sensor_color_t & Color);

int anderemain(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
  BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

  sensor_color_t      Color1;
  sensor_light_t      Light1;

  while(true){

    bool black_line_right = lightsensorBlackLineDetect(Light1);
    bool black_line_left = colorsensorBlackLineDetect(Color1);
    cout << "Zwarte lijn right: " << black_line_right;
    cout << "Zwarte lijn Links: " << black_line_left;
    cout << "....." << endl;



    sleep(1);
  }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

bool lightsensorBlackLineDetect(sensor_light_t  & light1) {
    if (BP.get_sensor(PORT_3, light1) == 0) {
        if (light1.reflected < 2400) {
            return false;
        } else if (light1.reflected >= 2400) {
            return true;
        }
//        cout << "Light sensor (S3): reflected " << setw(4) << light1.reflected << endl;
    }
}

bool colorsensorBlackLineDetect(sensor_color_t & Color1) {
    if (BP.get_sensor(PORT_1, Color1) == 0) {
        int color_rgb_sensor = Color1.color;
        switch (color_rgb_sensor) {
            case 1:
                return true;
            case 2:
                return false;
            case 3:
                return false;
            case 4:
                return false;
            case 5:
                return false;
            case 6:
                return false;
        }
    }
}

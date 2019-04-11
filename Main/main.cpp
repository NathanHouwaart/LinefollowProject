/*
---------------------------------------------------------------------------------------------
-----Created by TICT-1C groep 1                                                         -----
-----Lego mindstorms linefollower robot project                                         -----
-----Contributors:                                                                      -----
-----Jochem van Weelde, Stijn van Wijk, Wietse ten Dam, Kevin Patist & Nathan Houwaart  -----
 ---------------------------------------------------------------------------------------------
*/

#include "linefollower.h"

using namespace std;

void exit_signal_handler(int signo);
BrickPi3 BP;

int main() {
    string modeString; // variable to save the input from user phone
    char mode_select; //variable to save the answer of the user
    bool correct_answer = false;

    /*-----Setup exit handler and detect BrickPi-----*/
    signal(SIGINT, exit_signal_handler);                        // Register the exit function for Ctrl+C & cleanup
    BP.detect();                                                // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    /*-----Set sensor types-----*/
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);      // Set port 1 to be read as a color sensor
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_COLOR_RED);      // Set port 3 to be read as a color sensor
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);     // Set port 4 to be read as a ultrasonic sensor

    /*-----Set the lcd pins and the start the lcd------*/
    if (wiringPiSetup () == -1) {
        cout << "wiringpisetup did not work" << endl << "return 1;" << endl;
        return 1;
    }
    int fd = wiringPiI2CSetup(I2C_ADDR);
    lcdStart(fd);   // setup LCD
    clearLcd(fd);   // clear the lcd
    cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
    typeString("Setting up", fd);   // print the text on the screen

    /*-----Set data structs-----*/
    sensor_color_t Color1;                                      // Initialise struct for data storage color sensor 1
    sensor_color_t Color2;                                      // Initialise struct for data storage color sensor 2
    sensor_ultrasonic_t UltraSonic1;

    if(!checkVoltage(BP)) return 0;                    // Checks whether battery has enough power

    /*-----Calibrate min and max reflection values and determine lightvalue the robot wants to follow-----*/
    CalculatingErrorData struct_line_values;

    clearLcd(fd);   // clear the lcd
    cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
    typeString("Calibrating", fd);   // print the text on the screen

    calibration(Color1, struct_line_values, BP);
    defineDifferenceToAverage(struct_line_values);
    sleep(1); //Waiting for sensors to see normally
    clearLcd(fd);           // Clear the lcd

    /*-----setting up a bluetooth connection-----*/
    BluetoothServerSocket serversock(2, 1); //the channel number is 2
    cout << "listening" << endl;
    BluetoothSocket* clientsock = serversock.accept();
    cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;

    while(!correct_answer){
        cout << "Select mode: Line follow (L) / grid follow (G) / Free ride (F)" << endl;
        cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
        typeString("Select mode:", fd);  // print the text on the screen
        cursorLocation(LINE2, fd);      // set the cursorlocation to line 2
        typeString("L G F", fd);        // print the text to the screen

        while(mb.isRunning()) {
            input = mb.readMessage();  //blokkeert niet
            if(input == "A") {
                cout << input << endl;
                inputC = 'L';
                break;
            } else if (input == "B") {
                cout << input << endl;
                inputC = 'G';
                break;
            } else if (input == "C") {
                cout << input << endl;
                inputC = 'F';
                break;
            }
            cout << ".";
            cout.flush();
            usleep(300*1000);
        }

        switch (mode_select) {
            case 'L':
                cout << "Entering the line follow-mode." << endl;
                lineFollowLoop(Color1, Color2, UltraSonic1, struct_line_values, fd, clientsock, BP);
                break;
            case 'G':
                cout << "Entering the grid navigate-mode." << endl;
                gridFollowLoop(Color1, Color2, UltraSonic1, struct_line_values, fd, clientsock, BP);
                break;
            case 'F':
                cout << "Entering the freeride-mode." << endl;
                freeRideLoop(fd, clientsock, BP);
                break;
            default:
                cout << "ERROR, wrong input" << endl;
                break;
        }
   }
}

void exit_signal_handler(int signo) {
    /*-----This code will run when exiting the program with ctr + c. Used to reset de BrickPi outputs-----*/
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}

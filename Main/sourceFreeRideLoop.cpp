#include "linefollower.h"

using namespace std;

// TODO: --> Maak loop van de modus freeRideLoop() (Zie activity diagram)

void freeRideLoop(int & fd, BluetoothSocket* clientsock, BrickPi3 & BP){
    int playing = 0;
    int lcd_counter = 10000;		// The counter to keet the lcd fomr opdating every loop and starts at 10000 to get a dislpay
    while(true) {
        lcd_counter++;                      // every loop add 1 one to the counter
        if (lcd_counter >= 5000) {          // after 5000 loops its updates the screen
            float battery = BP.get_voltage_battery();
            float battery_percentage = (100 / (12.6 - 10.8) * (battery - 10.8));
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeFloat(battery_percentage, fd);  // display the battery_percantage
            cursorLocation(LINE2, fd);     // set the cursorlocation to line 2
            typeString("PCT   Freeride", fd);   // print the text on the screen
            lcd_counter = 0;                    // reset the counter
        }
        MessageBox& mb = clientsock->getMessageBox();

        float steer = 1;
        string input;
        while(mb.isRunning()) {
            input = mb.readMessage();  //Putting bluetooth input into a variable
            if(input != "") {
                cout << endl << input << endl;
                if (input == "C") {
                    return 0;
                }
                selectDirection(input, BP, steer, playing);
            }
            cout << ".";
            cout.flush();
            usleep(200000); // wait 200 ms
        }
        clientsock->close();
    }
        
}

void selectDirection(const string & input, BrickPi3 & BP, float & steer, int & playing) {

    if (input == "UP") {
        playSound('F', playing);
        steer = 1;
        drive(1, 100, 500, BP);
    } else if (input == "DOWN") {
        playSound('B', playing);
        steer = 1;
        drive(-2, 100, 500, BP);
    } else if (input == "FIRE") {
        stopSound(playing);
	drive(-1, 100, 500, BP);
    } else if (input == "LEFT") {
        playSound('F', playing);
        steer -= 0.2;
        if(steer <= 0){
            steer = 0;
        }
        drive(steer, 100, 500, BP);
    } else if (input == "RIGHT") {
        playSound('F', playing);
        steer += 0.2;
        if(steer >= 2){
            steer = 2;
        }
        drive(steer, 100, 500, BP);
    } else {
        int playing = 0;
        if (input == "A") {
            playSound('T', playing);
        }else if (input == "B"){
            playSound('X', playing);
        }else if (input == "C"){
            playSound('C', playing);
        }
    }
}

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

void freeRideLoop(int & address_lcd, BrickPi3 & BP){
    /* This is the main loop that we use to drive the robot with remote control.
     * The remote control is over bluetooth, you connected with an android-phone were the correct app is installed.
     *
     */
    int playing = 0;
    BluetoothServerSocket serversock(2, 1);                 //2 is het channel-number
    cout << "listening" << endl;
    int lcd_counter = 5000;		                            // The counter to keet the lcd fomr opdating every loop and starts at 10000 to get a dislpay
    while(true) {
        lcd_counter++;                                      // every loop add 1 one to the counter
        if (lcd_counter >= 5000) {
            // after 5000 loops the screen is updated
            printPercentage(address_lcd,'F',BP);            // Print the new battery percentage
            lcd_counter = 0;                                // reset the counter
        }
        BluetoothSocket* clientsock = serversock.accept();
        cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
        MessageBox& mb = clientsock->getMessageBox();

        float steer = 1;
        string input;
        while(mb.isRunning()) {
            input = mb.readMessage();  //blokkeert niet
            if(input != "") {
                cout << endl << input << endl;
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

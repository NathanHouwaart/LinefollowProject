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

void freeRideLoop(int & fd_lcd, BrickPi3 & BP){
    /* This is the main loop that we use to drive the robot with remote control.
     * The remote control uses bluetooth, you connected with an android-phone were the correct app is installed.
     * The function waits for messages from the message box.
     */
    int playing = 0;                                                    // Used to play sound, 0 means no sound is playing
    int lcd_counter = 5000;		                                         // To keep the lcd form updating every loop (updated every 5000 loops)
    BluetoothServerSocket serversock(2, 1);                             // 2 is het channel-number
    cout << "Listening for bluetooth connection" << endl;

    /*-----This while only stops if you close the program (ctrl+c)-----*/
    while(true) {
        lcd_counter++;                                                  // Every loop add 1 one to the counter
        if (lcd_counter >= 5000) {
            // After 5000 loops the screen is updated
            printPercentage(fd_lcd,'F',BP);                             // Print the new battery percentage
            lcd_counter = 0;                                            // Reset the counter
        }
        BluetoothSocket* clientsock = serversock.accept();              // Initialising the bluetooth connection
        cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
        MessageBox& mb = clientsock->getMessageBox();                   // Initialise the message box

        float steer = 1;                                                // The value that decides the direction the robot drives
        string input_message_box;                                       // String to save the input given from the messagebox
        /*-----The loop is active while the message box is running-----*/
        while(mb.isRunning()) {
            input_message_box = mb.readMessage();                       // Get the bluetooth input from message box
            if(input_message_box != "") {
                // Input is not empty so input is send to the direction control
                cout << endl << input_message_box << endl;              // Print out the current input
                selectDirection(input_message_box, steer, playing, BP); // Send the input to the steer function
            }
            cout << ".";                                                // Places a dot for every given input (counter)
            cout.flush();                                               // Forces the cout buffer to the output
            usleep(1000*200);                                           // Wait 200 ms
        }
        clientsock->close();                                            // Close the bluetooth socket when message box stopped running
    }
}

void soundSelection(const string & input, int & playing){
    /* This function is used to react on the 3 extra buttons that are in the bluetooth app.
     * It expects a string as input, this can either be "A", "B" or "C".
     * The input is a string because the message box returns a string.
     */
    playing = 0;
    if (input == "A") {
        // Plays the sound we have allocated to button A
        playSound('T', playing);
    }else if (input == "B"){
        // Plays the sound we have allocated to button B
        playSound('X', playing);
    }else if (input == "C"){
        // Plays the sound we have allocated to button C
        playSound('C', playing);
    }
}

void selectDirection(const string & input, float & steer, int & playing, BrickPi3 & BP) {
    /* The functions expects a string, this is the input. Based on the input we decide what action follows.
     * The steer variable controls the direction of the robot, expect when the direction is backwards.
     * The playing variable is needed for playing sounds.
     */
    if (input == "UP") {
        // The robot drives straight forward
        playSound('F', playing);
        steer = 1;
        drive(steer, 100, 500, BP);                 // Call the function that drives the robot
    } else if (input == "DOWN") {
        // The robot drives backwards
        playSound('B', playing);
        steer = 1;
        drive(-2, 100, 500, BP);                    // Call the function that drives the robot
    } else if (input == "FIRE") {
        // The robot stops on the spot
        stopSound(playing);
        steer = 1;
	    drive(-1, 100, 500, BP);                    // Call the function that drives the robot
    } else if (input == "LEFT") {
        // The robot steers to the left
        playSound('F', playing);
        steer -= 0.2;                               // Gradually decrease the turning radius of the robot to the left
        if(steer <= 0){
            // Makes sure that steer stays in the given boundaries of the drive function (0 => steer =< 2)
            steer = 0;
        }
        drive(steer, 100, 500, BP);                 // Call the function that drives the robot
    } else if (input == "RIGHT") {
        // The robot steers to the right
        playSound('F', playing);
        steer += 0.2;                               // Gradually decrease the turning radius of the robot to the right
        if(steer >= 2){
            // Makes sure that steer stays in the given boundaries of the drive function (0 => steer =< 2)
            steer = 2;
        }
        drive(steer, 100, 500, BP);                 // Call the function that drives the robot
    } else {
        soundSelection(input, playing);
    }
}
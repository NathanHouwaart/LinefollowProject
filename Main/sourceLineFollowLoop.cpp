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

void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd_lcd,  BrickPi3 & BP){
    /*Line
     * This function runs when the Linemode is selected at startup.
     * This is the main linefollow function and keeps running forever.
     * The robot will follow a line, drive around an object, play music, detect a crossroad and display something on the LCD
     * CTRL + C will stop this loop.
     */
     char choice_dodge_object;
     int counter_obstacle_detect = 0;
     int playing = 0;                           // Telling the program that no sound is currently playing
     int lcd_counter = 5000;                    // to keep the lcd form updating every loop (updated every 5000 loops)
     BluetoothServerSocket serversock(2, 1);    // The channel number is 2
     cout << "listening" << endl;
     BluetoothSocket* clientsock = serversock.accept();
     cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
     
     while (true) {
        lcd_counter++;                          // Add one to the counter for every loop
        if (lcd_counter >= 5000) {              // After 5000 loops update the LCD
            printPercentage(fd_lcd,'L',BP);
            lcd_counter = 0;                    // Reset the counter
        }
        BP.get_sensor(PORT_1, Color1);          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);          // Read colorsensor2 and put data in struct Color2
        int main_sensor_measurement = Color1.reflected_red;

        if(main_sensor_measurement < data_struct.lowest_measurement){
            // Checks if current reading is bigger or smaller than the highest or lowest value, if true then calculate new average
            data_struct.lowest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurement > data_struct.highest_measurement){
            data_struct.highest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 20) {
            // US bigger than 20 so check crossroad and line follow
            playSound('F', playing);                            // Play forward sound
            counter_obstacle_detect = 0;
            if (Color2.reflected_red < data_struct.average_min_max && main_sensor_measurement < data_struct.average_min_max) {
                // Crossroad detected because both sensors lower than average
                playSound('C', playing);                        // Play Crossroad detected sound
                crossroad(playing, fd_lcd, clientsock, BP);         // Run Crossroad function so the robot can go left/right/forward
                lcd_counter = 5000;                             // Restart the lcd and give the battery percentage
            } else {
                // If no intersection was detected, follow the line
                int error_to_average = defineError(data_struct.average_min_max, data_struct.difference_min_average, data_struct.difference_max_average, main_sensor_measurement);
                pController(error_to_average, BP);              // Drive with the PID controller
            }
        } else {
            // If an object was detected within X cm, execute this code
            playSound('O', playing);
            drive(DIRECTION_STOP, 0, 360, BP);                  // Stop the robot
            counter_obstacle_detect++;
            if (counter_obstacle_detect >= 500) {
                // after 500 consecutive readings the robot enters this code
                cout << "YEEBUG: I am in the obstacle detect." << endl;
                counter_obstacle_detect = 0;                    // Makes sure we detect the next object correctly with the buffer
                bool correct_answer = false;                    // Makes it able to loop the question if the answer is incorrect

                while(!correct_answer){
                    /*-----Ask for input until a correct char is given-----*/
                    cout << "Object detected: What do you want to do, type: D(dodge) or X(Plan X)" << endl;
                    cin >> choice_dodge_object;
                    switch (choice_dodge_object) {
                        case 'X':
                            // Starts plan X, charging add the robot
                            megaCharge(playing, BP);
                            correct_answer = true;
                            break;
                        case 'D':
                            // Starts dodging the object
                            playSound('D', playing);
                            driveAroundObject(UltraSonic, Color1, Color2, data_struct.average_min_max, BP);
                            correct_answer = true;
                            break;
                        default:
                            cout << "Wrong input. Please try again";
                            break;
                    }
                }
            }
        }
    }
}

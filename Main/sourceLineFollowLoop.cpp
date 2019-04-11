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

// TODO: --> Mogelijke aanpassing door verandering object ontwijken.
// TODO: --> Meting lager of hoger dan min of max meting (calibratie) Waarde laag en hoog aanpassen.

char choice_dodge_object;
int counter_obstacle_detect = 0;
int playing = 0;        //telling the program that no sound is currently playing
int lcd_counter = 5000;    // to keep the lcd form updating every loop and than noging shows and start a 10000 to start the lcd

void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd,  BrickPi3 & BP){
    /*
     * This function runs when the Linemode is selected at startup.
     * This is the main linefollow function and keeps running forever.
     * The robot will follow a line, drive around an object, play music, detect a crossroad and display something on the LCD
     * CTRL + C will stop this loop.
     */
     char choice_dodge_object;
     int counter_obstacle_detect = 0;
     int playing = 0;                           // Telling the program that no sound is currently playing
     int lcd_counter = 5000;                    // To keep the lcd form updating every loop and than noging shows and start a 10000 to start the lcd
     BluetoothServerSocket serversock(2, 1);    // The channel number is 2
     cout << "listening" << endl;
     BluetoothSocket* clientsock = serversock.accept();
     cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
     
     while (true) {
        lcd_counter++;                          // Add one to the counter for every loop
        if (lcd_counter >= 5000) {              // After 5000 loops update the LCD
            printPercentage(fd,'L',BP);
            lcd_counter = 0;                    // Reset the counter
        }
        BP.get_sensor(PORT_1, Color1);          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);          // Read colorsensor2 and put data in struct Color2
        int main_sensor_measurment = Color1.reflected_red;
        
        // Checks if current reading is bigger or smaller than the highest or lowest value, if true then calculate new average
        if(main_sensor_measurment < data_struct.lowest_measurment){
            data_struct.lowest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurment > data_struct.highest_measurment){
            data_struct.highest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 20) {
            // US bigger than 10 so check crossroad and line follow
            playSound('F', playing);
            counter_obstacle_detect = 0;
            if (Color2.reflected_red < data_struct.avarage_min_max && main_sensor_measurment < data_struct.avarage_min_max) {
                // Crossroad detected because both sensors lower than average
                playSound('C', playing);
                crossroad(BP, playing, fd, clientsock);
                lcd_counter = 5000;                             // to restart the lcd and give the battery percantage
            } else {
                // If no intersection was detected, follow the line
                int error_to_avarage = defineError(data_struct.avarage_min_max, data_struct.difference_min_avarage, data_struct.difference_max_avarage, main_sensor_measurment);
                pController(error_to_avarage, BP);
            }
        } else {
            // If an object was detected within X cm, execute this code
            playSound('S', playing);
            drive(DIRECTION_STOP, 0, 360, BP);                  // Stop the robot
            counter_obstacle_detect++;
            if (counter_obstacle_detect >= 500) {
                // after 500 consecutive readings the robot enters this code
                cout << "YEEBUG: I am in the obstacle detect." << endl;
                counter_obstacle_detect = 0;                    // Makes sure we detect the next object correctly with the buffer
                playSound('D', playing);
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
                            driveAroundObject(UltraSonic, Color1, Color2, data_struct.avarage_min_max, BP);
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

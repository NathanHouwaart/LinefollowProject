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

void lineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BluetoothSocket* clientsock,  BrickPi3 & BP){
     while (true) {
        lcd_counter++;                  // add one to the counter for every loop
        if (lcd_counter >= 5000) {      // after 5000 loops update hij het schermpje
            float battery = BP.get_voltage_battery();
            float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeFloat(battery_percentage, fd);  // display the battery_percantage
            cursorLocation(LINE2, fd);     // set the cursorlocation to line 2
            typeString("PCT   Linefollow", fd);   // print the text on the screen
            lcd_counter = 0;                    // reset the counter
        }
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);
        int main_sensor_measurment = Color1.reflected_red;
        // Checks if current reading is bigger or smaller than the highest or lowest value, if true than calculate new average
        if(main_sensor_measurment < data_struct.lowest_measurment){
            data_struct.lowest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurment > data_struct.highest_measurment){
            data_struct.highest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 10) { // US bigger than 10 so check crossroad and line follow
            playSound('F', playing);
            counter_obstacle_detect = 0;
            // Crossroad detected because both sensors lower than average
            if (Color2.reflected_red < data_struct.avarage_min_max && main_sensor_measurment < data_struct.avarage_min_max) {
                playSound('C', playing);
                crossroad(BP, playing, fd, clientsock);
                lcd_counter = 5000;       // to restart the lcd and give the battery percantage
            } else {                                             // If no intersection was detected, follow the line
                int error_to_avarage = defineError(data_struct.avarage_min_max, data_struct.difference_min_avarage, data_struct.difference_max_avarage, main_sensor_measurment);
                pController(error_to_avarage, BP);
            }
        } else {                                    // If an object was detected within X cm, execute this code
            playSound('S', playing);
            drive(DIRECTION_STOP, 0, 360, BP);      // Stop the robot
            counter_obstacle_detect++;
            if (counter_obstacle_detect >= 500) {   // after 500 consecutive readings the robot enters this code
                cout << "YEEBUG: I am in the obstacle detect." << endl;
                counter_obstacle_detect = 0; // Makes sure we detect the next object correctly with the buffer
                bool correct_answer = false; // Makes it able to loop the question if the answer is incorrect

                while(!correct_answer){
                    cout << "Object detected: What do you want to do, type: D(dodge) or X(Plan X)" << endl;
                    cin >> choice_dodge_object;
                    switch (choice_dodge_object) {
                        case 'X':           // Starts plan X, charging add the robot.
                            megaCharge(playing, BP);
                            correct_answer = true;
                            break;
                        case 'D':           // Starts dodging the object
                            playSound('D', playing);
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

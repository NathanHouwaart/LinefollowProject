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


//int counter_object = 0;
//int playing = 0;        //telling the program that no sound is currently playing
//int lcd_counter = 10000;    // to keep the lcd form updating every loop and than noging shows and start a 10000 te start the lcd


void PIDlineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP, int & fd) {
    int counter_obstacle_detect = 0;
    char choice_dodge_object;
    int counter_object = 0;
    int playing = 0;        //telling the program that no sound is currently playing
    int lcd_counter = 10000;    // to keep the lcd form updating every loop and than noging shows and start a 10000 te start the lcd
    BluetoothServerSocket serversock(2, 1); //the channel number is 2
    cout << "listening" << endl;
    BluetoothSocket* clientsock = serversock.accept();
    cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;

    float target_power = 40;                                // Constant value to determine maximum motor dps
    float kp = 100.0/data_struct.difference_min_avarage *0.7;                                        // 100/ (((200+680)/2) - 200)     W 0,434 Z 0,819
    float kd = 15*kp; //(kp*0.1)/(20*0.00001);
    float ki = 0;

    int lastError = 0;
    int integral = 0;
    int offset = data_struct.avarage_min_max;           // Target ligh value for the robot to follow

    float turn_modifier = -2.0 * target_power / 100;


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

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 20) {       // If the measured US distance is bigger than 10:
            counter_object = 0;
            if (Color2.reflected_red < data_struct.avarage_min_max && main_sensor_measurment < data_struct.avarage_min_max) {
                playSound('C', playing);
                crossroad(BP, playing, fd, clientsock);
                lcd_counter = 100000;       // to restart the lcd and give the battery percantage
            } else {                                             // If no intersection was detected, follow the line
                PController(Color1, BP, data_struct, target_power, kp, kd, ki, lastError, integral, offset, turn_modifier);
            }
        } else {                                                  // If an object was detected within X cm, execute this code
            stopMotor(BP);      // Stop the robot
            counter_obstacle_detect++;
            if (counter_obstacle_detect >= 1000) {   // after 500 consecutive readings the robot enters this code
                cout << "YEEBUG: I am in the obstacle detect." << endl;
                counter_obstacle_detect = 0; // Makes sure we detect the next object correctly with the buffer
                playSound('D', playing);
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
                            driveAroundObject(UltraSonic, Color1, Color2, data_struct.avarage_min_max, BP);
                            correct_answer = true;
                            break;
                        default:
                            cout << "Wrong input. Please try again";
                            break;
                    }
                }
            }//Start driving around milk
        }
    }
}

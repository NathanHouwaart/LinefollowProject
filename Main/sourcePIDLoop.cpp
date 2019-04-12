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


void PIDlineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP, int & fd_lcd) {
    /*
     * This function runs when the LineFollow PID is selected at startup.
     * This is the main linefollow function and keeps running forever.
     *  The robot will follow a line, drive around an object, play music, detect a crossroad and display something on the LCD
     * CTRL + C will stop this loop.
     */
    int counter_obstacle_detect = 0;
    char choice_dodge_object;
    int counter_object = 0;
    int playing = 0;                            //telling the program that no sound is currently playing
    int lcd_counter = 10000;                    // to keep the lcd form updating every loop and than noging shows and start a 10000 te start the lcd
    BluetoothServerSocket serversock(2, 1);     //the channel number is 2
    cout << "listening" << endl;
    BluetoothSocket* clientsock = serversock.accept();
    cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;

    float target_power = 40;                            // Constant value to determine maximum motor dps
    float kp = 0.80;                                    //100.0/data_struct.difference_min_average *0.8;                                        // 100/ (((200+680)/2) - 200)     W 0,434 Z 0,819

    float kd = (kp*0.1)/(8*0.00001);

    float ki = 0;

    int lastError = 0;
    int integral = 0;
    int offset = data_struct.average_min_max;           // Target ligh value for the robot to follow

    float turn_modifier = -2.0 * target_power / 100;


    while (true) {
        lcd_counter++;                                          // Add one to the counter for every loop
        if (lcd_counter >= 5000) {                              // After 5000 loops update hij het schermpje
            printPercentage(fd_lcd,'P',BP);
            lcd_counter = 0;                                    // Reset the counter
        }
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);
        int main_sensor_measurement = Color1.reflected_red;
        if(main_sensor_measurement < data_struct.lowest_measurement){
            data_struct.lowest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurement > data_struct.highest_measurement){
            data_struct.highest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 20) {       // If the measured US distance is bigger than 10:
            playSound('F', playing);
            counter_object = 0;
            cout << "hier ben ik" << endl;
            if (Color2.reflected_red < data_struct.average_min_max && main_sensor_measurement < data_struct.average_min_max) {
                cout << "ik ben er on" << endl;
                playSound('C', playing);
                crossroad(playing, fd_lcd, clientsock, BP);
                lcd_counter = 100000;       // to restart the lcd and give the battery percantage
            } else {                                             // If no intersection was detected, follow the line
                PController(Color1, BP, data_struct, target_power, kp, kd, ki, lastError, integral, offset, turn_modifier);
            }
        } else {                                                  // If an object was detected within X cm, execute this code
            stopMotor(BP);      // Stop the robot
            counter_obstacle_detect++;
            if (counter_obstacle_detect >= 500) {   // after 500 consecutive readings the robot enters this code
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

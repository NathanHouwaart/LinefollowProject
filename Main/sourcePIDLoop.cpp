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


int counter_object = 0;
int playing = 0;        //telling the program that no sound is currently playing
int lcd_counter = 10000;    // to keep the lcd form updating every loop and than noging shows and start a 10000 te start the lcd


void PIDlineFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP, int & fd) {
    int counter_object = 0;
    int playing = 0;        //telling the program that no sound is currently playing
    int lcd_counter = 10000;    // to keep the lcd form updating every loop and than noging shows and start a 10000 te start the lcd
    BluetoothServerSocket serversock(2, 1); //the channel number is 2
    cout << "listening" << endl;
    BluetoothSocket* clientsock = serversock.accept();
    cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
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
        if(main_sensor_measurment < data_struct.lowest_measurment){
            data_struct.lowest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurment > data_struct.highest_measurment){
            data_struct.highest_measurment = main_sensor_measurment;
            defineDifferenceToAverage(data_struct);
        }

        if (getUltraSValue(PORT_4, UltraSonic, BP) > 10) {       // If the measured US distance is bigger than 10:
            playSound('F', playing);
            counter_object = 0;
            if (Color2.reflected_red < data_struct.avarage_min_max && main_sensor_measurment < data_struct.avarage_min_max) {
                playSound('C', playing);
                crossroad(BP, playing, fd, clientsock);
                lcd_counter = 100000;       // to restart the lcd and give the battery percantage
            } else {                                             // If no intersection was detected, follow the line
                PController(Color1, BP, data_struct);
            }
        } else {                                                  // If an object was detected within X cm, execute this code
            playSound('O', playing);
            stopMotor(BP); // Stop the car
            counter_object++;
            if (counter_object % 1000 == 0) { cout << "Joe 1000 iets" << counter_object << endl; }
            if (counter_object >= 1500) {
                cout << "Counter is groot genoeg" << endl;
                //playSound('D', playing);
                //driveAround(BP);
                megaCharge(playing, BP);
            }//Start driving around milk
        }
    }
}

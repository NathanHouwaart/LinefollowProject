#include "linefollower.h"

using namespace std;

// TODO: --> Maak een loop voor het volgen van een grid. (Zie activity diagram)

void gridFollowLoopPID(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP){

    unsigned int width;
    unsigned int height;
    int direction_index = -1;
    char seperator, facing_direction;

 /*
    cout << "Looking right" << endl;
    lookRight(UltraSonic, BP);
    usleep(1000*1000);
    cout << "Looking Left" << endl;
    lookLeft(UltraSonic, BP);
    usleep(1000*1000);
*/

    float target_power = 40;                                // Constant value to determine maximum motor dps
    float kp = 100.0/data_struct.difference_min_average *0.8;                                        // 100/ (((200+680)/2) - 200)     W 0,434 Z 0,819

    float kd = (kp*0.1)/(20*0.00001);

    float ki = 0;

    int lastError = 0;
    int integral = 0;
    int offset = data_struct.average_min_max;           // Target ligh value for the robot to follow

    float turn_modifier = -2.0 * target_power / 100;
    
    cout << "Welcome in GRID mode!" << endl;
    cout << "What is the height and with of the grid? Format 'H, W':\t";
    cin >> height >> seperator >> width;
    cout << "What position is the robot currently facing? (D)own, (U)p, (L)eft, (R)ight:\t";
    cin >> facing_direction;

    vector<vector<char>> grid = gridSetup(height, width);
    vector<char> fastest_route = fastestRoute(height, width);
    int lcd_counter = 100000;        // to keep the lcd form updating every loop and starts at 10000 to start the lcd configuration

    while (true) {
        lcd_counter++;          // add one to the counter
        if (lcd_counter >= 5000) {      // after every 5000 loops updates the lcd screen
            float battery = BP.get_voltage_battery();
            float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
            clearLcd(fd);   // clear the lcd
            cursorLocation(LINE1, fd);      // set the cursorlocation to line 1
            typeFloat(battery_percentage, fd);  // display the battery_percantage
            cursorLocation(LINE2, fd);     // set the cursorlocation to line 2
            typeString("PCT   Grid mode", fd);   // print the text on the screen
            lcd_counter = 0;        // rest the counter
        }
        int playing = 0;
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

        if (Color2.reflected_red < (data_struct.average_min_max) && main_sensor_measurement < data_struct.average_min_max) {
            direction_index += 1;
            if(direction_index >= fastest_route.size()){
                break;
            }else{
                vector<size_t> position = getRobotPosition(grid);
                stopMotor(BP);
                char look_direction = relativeDirection(facing_direction, fastest_route[direction_index]);
                cout << "Facing direction: " << facing_direction << endl;
                cout << "Absolute direction: " << facing_direction << endl;
                cout << "robot look direction: " << look_direction << endl;
                whereToLook(grid, look_direction, facing_direction, position, UltraSonic, BP);
                printGrid(grid);

                updateRobotPosition(grid, fastest_route[direction_index], fastest_route, direction_index);
                char robot_instruction = relativeDirection(facing_direction, fastest_route[direction_index]);
                updateRobotOrientation(facing_direction, fastest_route[direction_index]);
                crossroadGrid(BP, robot_instruction, playing, fd);
                lcd_counter = 10000;        // to get the lcd screen back to the main version
            }
        } else {                                             // If no intersection was detected, follow the line
            PController(Color1, BP, data_struct, target_power, kp, kd, ki, lastError, integral, offset, turn_modifier);
        }
    }
    cout << "Finished" << endl;
    BP.reset_all();
}

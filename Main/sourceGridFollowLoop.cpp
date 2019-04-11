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

void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct, int & fd, BrickPi3 & BP){

    unsigned int width;
    unsigned int height;
    int direction_index = -1;
    char seperator, facing_direction;

    cout << "Welcome in GRID mode!" << endl;
    cout << "What is the height and with of the grid? Format 'H, W':\t";
    cin >> height >> seperator >> width;
    cout << "What position is the robot currently facing? (D)own, (U)p, (L)eft, (R)ight:\t";
    cin >> facing_direction;

    vector<vector<char>> grid = gridSetup(height, width);
    vector<char> fastest_route = fastestRoute(height, width);
    int lcd_counter = 100000;                           // To keep the lcd form updating every loop and starts at 10000 to start the lcd configuration

    while (true) {
        lcd_counter++;                                  // Add one to the counter
        if (lcd_counter >= 5000) {                      // After every 5000 loops updates the lcd screen
            float battery = BP.get_voltage_battery();
            float battery_percentage = (100/(12.6-10.8)*(battery-10.8));
            clearLcd(fd);                               // Clear the lcd
            cursorLocation(LINE1, fd);                  // Set the cursorlocation to line 1
            typeFloat(battery_percentage, fd);          // Display the battery_percantage
            cursorLocation(LINE2, fd);                  // Set the cursorlocation to line 2
            typeString("PCT   Grid mode", fd);          // Print the text on the screen
            lcd_counter = 0;                            // Rest the counter
        }
        int playing = 0;
        BP.get_sensor(PORT_1, Color1);                  // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);                  // Read Colorsensor2 and put data in struct Color2
        int main_sensor_measurement = Color1.reflected_red;
        if(main_sensor_measurement < data_struct.lowest_measurement){
            data_struct.lowest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurement > data_struct.highest_measurement){
            data_struct.highest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        }

        if (Color2.reflected_red < (data_struct.avarage_min_max) && main_sensor_measurement < data_struct.avarage_min_max) {
            direction_index += 1;
            if(direction_index >= fastest_route.size()){
                break;
            }else{
                vector<size_t> position = getRobotPosition(grid);
                drive(DIRECTION_STOP, 0, 360, BP);
                char look_direction = relativeDirection(facing_direction, fastest_route[direction_index]);
                whereToLook(grid, look_direction, facing_direction, position, UltraSonic, BP);
                printGrid(grid);

                updateRobotPosition(grid, fastest_route[direction_index], fastest_route, direction_index);
                char robot_instruction = relativeDirection(facing_direction, fastest_route[direction_index]);
                updateRobotOrientation(facing_direction, fastest_route[direction_index]);
                crossroadGrid(BP, robot_instruction, playing, fd);
                lcd_counter = 10000;                        // to get the lcd screen back to the main version
            }
        } else {
            // If no intersection was detected, follow the line
            int error_to_average = defineError(data_struct.avarage_min_max, data_struct.difference_min_avarage, data_struct.difference_max_avarage,
                                              main_sensor_measurement);
            pController(error_to_average, BP);
        }
    }
    cout << "Finished" << endl;
    BP.reset_all();
}

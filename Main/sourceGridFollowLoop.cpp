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
    int lcd_counter = 5000;                             // Keep the lcd form updating every loop (updated every 5000 loops)

    while (true) {
        lcd_counter++;                                  // Add one to the counter
        if (lcd_counter >= 5000) {                      // After every 5000 loops updates the lcd screen
            printPercentage(fd,'G',BP);
            lcd_counter = 0;                            // Reset the counter
        }
        int playing = 0;
        BP.get_sensor(PORT_1, Color1);                  // Read Colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);                  // Read Colorsensor2 and put data in struct Color2
        int main_sensor_measurement = Color1.reflected_red;
        if(main_sensor_measurement < data_struct.lowest_measurement){
            // If the sensor sees a lower value than the lowest value, we want to calculate the average again so we cant get false values
            data_struct.lowest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        } else if(main_sensor_measurement > data_struct.highest_measurement){
            // If the sensor sees a higher value than the highest value, we want to calculate the average again so we cant get false values
            data_struct.highest_measurement = main_sensor_measurement;
            defineDifferenceToAverage(data_struct);
        }

        if (Color2.reflected_red < (data_struct.average_min_max) && main_sensor_measurement < data_struct.average_min_max) {
            // If the robot detects a crossroad
            direction_index += 1;
            if(direction_index >= fastest_route.size()){
                // If the robot is at the end of his route
                break;
            }else{
                // If the robot is not at the end of his route
                vector<size_t> position = getRobotPosition(grid);                                               // Store the current position
                drive(DIRECTION_STOP, 0, 360, BP);                                                              // Stop the car
                char look_direction = relativeDirection(facing_direction, fastest_route[direction_index]);      // Get the direction the robot is looking
                whereToLook(grid, look_direction, facing_direction, position, UltraSonic, BP);                  // Determine where the robot needs to look
                printGrid(grid);                                                                                // Print the grid for visualisation

                updateRobotPosition(grid, fastest_route[direction_index], fastest_route, direction_index);      // Update the robot's position
                char robot_instruction = relativeDirection(facing_direction, fastest_route[direction_index]);   // Determine the instruction where the robot needs to go
                updateRobotOrientation(facing_direction, fastest_route[direction_index]);                       // Update the robot's facing direction
                crossroadGrid(BP, robot_instruction, playing, fd);                                              // The robot drives to the next crossroad
                lcd_counter = 10000;                                                                            // Get the lcd screen back to the main version
            }
        } else {
            // If no intersection was detected, follow the line
            int error_to_average = defineError(data_struct.average_min_max, data_struct.difference_min_average, data_struct.difference_max_average,
                                              main_sensor_measurement);
            pController(error_to_average, BP);                                                                  //Follow the line with a P controller
        }
    }
    cout << "Finished" << endl;
    BP.reset_all();
}

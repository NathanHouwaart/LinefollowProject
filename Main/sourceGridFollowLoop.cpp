#include "linefollower.h"

using namespace std;

// TODO: --> Maak een loop voor het volgen van een grid. (Zie activity diagram)

void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic1, vector<int> & min_max_reflection_value, vector<int> & default_values, BrickPi3 & BP){

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
    vector<char> fastest_route = fastestRoute(height, width, facing_direction, BP);

    while (true) {
        BP.get_sensor(PORT_1, Color1);                          // Read colorsensor1 and put data in struct Color1
        BP.get_sensor(PORT_3, Color2);
        if (Color2.reflected_red < (default_values[0]) && Color1.reflected_red < default_values[0]) {
            direction_index += 1;
            if(direction_index >= fastest_route.size()){
                break;
            }else{
                printGrid(grid);
                facing_direction = updateRobotPosition(grid, fastest_route[i], facing_direction);
                crossroad(BP, fastest_route[direction_index]);
            }
        } else {                                             // If no intersection was detected, follow the line
            int stuurwaarde = defineDirection(default_values[0], default_values[1], default_values[2],
                                              Color1.reflected_red);
            stuur(stuurwaarde, BP);
        }
    }
    cout << "Finished" << endl;
}

#include "linefollower.h"

using namespace std;

// TODO: --> Maak een loop voor het volgen van een grid. (Zie activity diagram)

void gridFollowLoop(sensor_color_t & Color1, sensor_color_t & Color2, sensor_ultrasonic_t & UltraSonic, CalculatingErrorData data_struct , BrickPi3 & BP){

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
    cout << "Welcome in GRID mode!" << endl;
    cout << "What is the height and with of the grid? Format 'H, W':\t";
    cin >> height >> seperator >> width;
    cout << "What position is the robot currently facing? (D)own, (U)p, (L)eft, (R)ight:\t";
    cin >> facing_direction;

    vector<vector<char>> grid = gridSetup(height, width);
    vector<char> fastest_route = fastestRoute(height, width);


    while (true) {
        int playing = 0;
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

        if (Color2.reflected_red < (data_struct.avarage_min_max) && main_sensor_measurment < data_struct.avarage_min_max) {
            direction_index += 1;
            if(direction_index >= fastest_route.size()){
                break;
            }else{
                vector<size_t> position = getRobotPosition(grid);
                drive(DIRECTION_STOP, 0, 360, BP);
                char look_direction = relativeDirection(facing_direction, fastest_route[direction_index]);
                cout << "Facing direction: " << facing_direction << endl;
                cout << "Absolute direction: " << facing_direction << endl;
                cout << "robot look direction: " << look_direction << endl;
                whereToLook(grid, look_direction, facing_direction, position, UltraSonic, BP);
                printGrid(grid);

                updateRobotPosition(grid, fastest_route[direction_index], fastest_route, direction_index);
                char robot_instruction = relativeDirection(facing_direction, fastest_route[direction_index]);
                updateRobotOrientation(facing_direction, fastest_route[direction_index]);
                crossroad(BP, robot_instruction, playing);
            }
        } else {                                             // If no intersection was detected, follow the line
            int error_to_average = defineError(data_struct.avarage_min_max, data_struct.difference_min_avarage, data_struct.difference_max_avarage,
                                              main_sensor_measurment);
            pController(error_to_average, BP);
        }
    }
    cout << "Finished" << endl;
    BP.reset_all();
}

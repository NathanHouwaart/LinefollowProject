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

char relativeDirection(const char & current_robot_orientation, const char & absolute_direction){
    /* This function translates the absolute directions given to relative directions the robot can follow */
    if(current_robot_orientation == absolute_direction){
        // If the facing direction is the same as the direction the robot needs to drive in:
        return 'F';                                                             // Forward
    }else{
        // If the facing direction is not the same as the direction the robot needs to drive in:
        switch (current_robot_orientation){
            case 'D':
                // If robot is oriented DOWN:
                if(absolute_direction =='R'){
                    // If robot needs to go to the right:
                    return 'L';                                                 // Robot needs to turn left
                }else if(absolute_direction == 'U'){
                    // If robot needs to go up
                    return 'B';                                                 // Robot needs to turn around and go backwards
                }else if(absolute_direction == 'L') {
                    // If robot needs to go to the left
                    return 'R';                                                 // Robot needs to turn right
                }
                break;
            case 'R':
                // If robot is orientated RIGHT
                if(absolute_direction =='U'){
                    // If robot needs to go up
                    return 'L';                                                 // Robot needs to turn left
                }else if(absolute_direction == 'L'){
                    // If robot needs to go to the Left
                    return 'B';                                                 // Robot needs to turn around and go backwards
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    return 'R';                                                 // Robot needs to turn right
                }
                break;
            case 'U':
                // If robot is oriented UP
                if(absolute_direction =='L'){
                    // If robot needs to go to the Left
                    return 'L';                                                 // Robot needs to turn left
                }else if(absolute_direction == 'R'){
                    // If robot needs to go to the right:
                    return 'R';                                                 // Robot needs to turn right
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    return 'B';                                                 // Robot needs to turn around and go backwards
                }
                break;
            case 'L':
                // If robot is oriented LEFT
                if(absolute_direction =='U'){
                    // If robot needs to go up
                    return 'R';                                                 // Robot needs to turn right
                }else if(absolute_direction == 'R'){
                    // If robot needs to go to the right:
                    return 'B';                                                 // Robot needs to turn around and go backwards
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    return 'L';                                                 // Robot needs to turn left
                }
                break;
        }
    }
}

void updateRobotOrientation(char & current_robot_orientation, const char & absolute_direction){
    /* This function translates the absolute directions given to relative directions the robot can follow */
    if(current_robot_orientation == absolute_direction){                //  If the facing direction is the same as the direction the robot needs to drive in:
        return;
    }else{
        switch (current_robot_orientation){
            case 'D':
                // If robot is oriented DOWN:
                if(absolute_direction =='R'){
                    // If robot needs to go to the right:
                    current_robot_orientation = absolute_direction;             // Orientation changes to right
                }else if(absolute_direction == 'U'){
                    // If robot needs to go up
                    current_robot_orientation = absolute_direction;             // Orientation changes to up
                }else if(absolute_direction == 'L') {
                    // If robot needs to go to the left
                    current_robot_orientation = absolute_direction;             // Orientation changes to left
                }
                break;
            case 'R':
                // If robot is orientated RIGHT
                if(absolute_direction =='U'){
                    // If robot needs to go up
                    current_robot_orientation = absolute_direction;             // Orientation changes to up
                }else if(absolute_direction == 'L'){
                    // If robot needs to go to the Left
                    current_robot_orientation = absolute_direction;             // Orientation changes to left
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    current_robot_orientation = absolute_direction;             // Orientation changes to down
                }
                break;
            case 'U':
                // If robot is oriented UP
                if(absolute_direction =='L'){
                    // If robot needs to go to the Left
                    current_robot_orientation = absolute_direction;             // Orientation changes to left
                }else if(absolute_direction == 'R'){
                    // If robot needs to go to the right:
                    current_robot_orientation = absolute_direction;             // Orientation changes to right
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    current_robot_orientation = absolute_direction;             // Orientation changes to down
                }
                break;
            case 'L':
                // If robot is oriented LEFT
                if(absolute_direction =='U'){
                    // If robot needs to go up
                    current_robot_orientation = absolute_direction;             // Orientation changes to up
                }else if(absolute_direction == 'R'){
                    // If robot needs to go to the right:
                    current_robot_orientation = absolute_direction;             // Orientation changes to right
                }else if(absolute_direction == 'D') {
                    // If robot needs to go down
                    current_robot_orientation = absolute_direction;             // Orientation changes to down
                }
                break;
        }
    }
}

uint64_t factorial(uint64_t n){
    /* Recursive function to calculate the factorial of n */
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

void updateRoute(vector<char> & fastest_route, const unsigned int & index, const char & new_direction, const bool & redirect){
    /* Function updates the absolute route the robot needs to take in order to avoid objects.
     * This is done by inserting extra steps if the robot comes by a barrier.
     */
    auto itPos = fastest_route.begin() + index;                                                         // Insert position is defined
    fastest_route.insert(itPos, new_direction);                                                         // New direction is inserted into vector
    if(new_direction == 'U' && redirect){                                                               // Special exeption if the robot needs to go around an object
        vector<char> redirection = {'R', 'R','D'};
        fastest_route.insert(fastest_route.begin()+index+1, redirection.begin(), redirection.end());    // New directions are also inserted in vector
        fastest_route.pop_back();                                                                       // Last value is pop-ed because new values are added
    }else if(new_direction == 'L' && redirect){
        vector<char> redirection = {'U', 'U','R', 'R', 'D','D'};
        fastest_route.insert(fastest_route.begin() + index+1, redirection.begin(), redirection.end());  // New directions are also inserted in vector
    }
    fastest_route.pop_back();                                                                           // Last value is pop-ed because new values are added

    cout << "updated route: ";
    for (int j = index+1; j < fastest_route.size() ; ++j) {
        cout << fastest_route[j] << " ";
    }

    cout << endl;
}

unsigned int possibleRoutes(const unsigned int & grid_height, const unsigned int & grid_width){
    /* The amount of possible routes is calculated using the combination calculation.
     * The formula for a combination of choosing r !unique! ways from n possibilities is:
     * nCr = ( n! ) / ( r! (n - r)!)
     */
    unsigned int total_routes;
    uint64_t n = grid_height + grid_width;
    uint64_t r = grid_height;
    uint64_t n_factorial = factorial(n);
    uint64_t r_factorial = factorial(r);
    uint64_t n_min_r_factorial = factorial(n-r);
    vector<int> i = {4,5};
    total_routes = n_factorial / (r_factorial * n_min_r_factorial);
    return total_routes;
}

vector<char> fastestRoute(const unsigned int & grid_height, const unsigned int & grid_width){
    /* Function to define the fastest route on a grid. In our case this is (assuming the robot is placed in the
     * upper right position of the grid and facing down) all the way down and then all the way to the right.
     * function returns a vector<char<> with robot instructions.
     */
    vector<char> fastest_route = {};                            // Initialise data struct
    for(unsigned int i = 0; i < grid_height; i++){
        fastest_route.push_back('D');                           // Go all the way down on the grid
    }
    for(unsigned int i = 0; i < grid_width; i++){
        fastest_route.push_back('R');                           // If robot is all the way down, go all the way to the right
    }
    return fastest_route;                                       // Return fastest route
}

vector<vector<char>> gridSetup(const unsigned int & grid_height, const unsigned int & grid_width){
    /* Function sets up a grid in a vector of a variable size with grid_height & grid_width.
     * Constructs a grid  with format:
     *  R - X - X
     *  |   |   |
     *  X - X - X
     *  |   |   |
     *  X - X - X
     *
     * With R being the current (start) robot position.
     * This grid can later be used to keep track of the objects the robot has seen on the grid by changing the - and | respectively.
     * It can also be used to update the current robot position.
     */

    vector<vector<char>> grid = {};                                 // Construct the grid vector

    for(int i = 0; i < (grid_height*2)+1; i++) {
        /* Setup x-coordinates constructor */
        vector<char> linevector = {};
        for (int j = 0; j < (grid_width*2)+1; j++) {
            /* Setup y-coordinates constructor */
            if(i == 0 && j == 0){
                // Set starting position of the robot
                cout << 'R';
                linevector.push_back('R');
            }else if(i%2 != 1 && j%2 != 1){
                // If i and j are even: construct a X
                cout << 'X';
                linevector.push_back('X');
            }else if(i%2 != 1 && j%2){
                // If i is even and j is uneven: construct a -
                cout << " - ";
                linevector.push_back('-');
            }else if(i%2 && j%2 != 1){
                // If i is uneven and j is even: construct a |
                cout << "|  ";
                linevector.push_back('|');
            }else{
                // Else construct a white space
                cout << ' ';
                linevector.push_back(' ');
            }
        }
        cout << endl;
        grid.push_back(linevector);                                 // Add current x - row to the grid
    }
    return grid;
}

void printGrid(const vector<vector<char>> & grid){
    /* Debug print to see if grid is constructed correctly */
    for(int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

vector<size_t> getRobotPosition(const vector<vector<char>> & grid){
    size_t x_coordinate = 0, y_coordinate = 0;
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 'R') {
                x_coordinate = j;        // x coordinate of R
                y_coordinate = i;        // y coordinate of R
            }
        }
    }
    return {x_coordinate, y_coordinate};
}

void updateRobotPosition(vector<vector<char>> & grid, const char & robot_direction, vector<char> & fastest_route, const unsigned int & index) {
    /* Updates the grid vector accordingly to the movement of the robot. Expects three parameters:
     * 1. The grid
     * 2. The direction the robot has driven
     * 3. the current facing direction of the robot.
     */
    vector<size_t> robot_position = getRobotPosition(grid);
    size_t y_coordinate = robot_position[1];
    size_t x_coordinate = robot_position[0];

    switch (robot_direction){
        case 'D':
            if(grid[y_coordinate + 2][x_coordinate] == '+'){
                if(grid[y_coordinate][x_coordinate+2] == '+'){
                    if(grid[y_coordinate-2][x_coordinate] == '+'){
                        grid[y_coordinate][x_coordinate-2] = 'R';
                        updateRoute(fastest_route, index, 'L', true);
                    }else{
                        grid[y_coordinate-2][x_coordinate] = 'R';
                        updateRoute(fastest_route, index, 'U', true);
                    }
                }else{
                    grid[y_coordinate][x_coordinate+2] = 'R';
                    updateRoute(fastest_route, index, 'R', false);
                }
            }else{
                grid[y_coordinate + 2][x_coordinate] = 'R';
                // position is still down
            }
            grid[y_coordinate][x_coordinate] = 'X';
            break;
        case 'U':
            grid[y_coordinate][x_coordinate] = 'X';
            grid[y_coordinate -2][x_coordinate] = 'R';
            break;
        case 'R':
            grid[y_coordinate][x_coordinate] = 'X';
            grid[y_coordinate][x_coordinate+2] = 'R';
            break;
        case 'L':
            grid[y_coordinate][x_coordinate] = 'X';
            grid[y_coordinate ][x_coordinate-2] = 'R';
            break;
    }
}


void updateBarrier(vector<vector<char>> & grid, const size_t & x_coordinate, const size_t & y_coordinate){
    /* Function to add a barrier to a grid. This is used so the robot can calculate whether or not it can pass through a point */
    grid[y_coordinate][x_coordinate] = '+';
}

bool lookRight(sensor_ultrasonic_t &  UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;                                      //Setting the motor to communicate
    int32_t turn = 95;                                                  // The degrees needed to turn the USsensor with gears
    BP.set_motor_limits(PORT_B, 100, 90);                               // Limit the motor so the gears can keep up
    BP.set_motor_position_relative(motor_middle, turn*-1);              // Turn  the motor -turn- degrees
    sleep(1);
    lookForward(UltraSonic, BP);
    sleep(1);
    BP.set_motor_position_relative(motor_middle, turn+10);              // Turn the motor back to original position
    sleep(1);
}

bool lookLeft(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP){
    uint8_t motor_middle = PORT_B;                                      // Setting the motor to communicate
    int32_t turn = 95;                                                  // The degrees needed to turn the motor Right
    BP.set_motor_limits(PORT_B, 100, 90);                               // Limit the motor so the gears can keep up
    BP.set_motor_position_relative(motor_middle, turn) ;                // Turn the motor -turn- degrees Right
    sleep(1);
    lookForward(UltraSonic, BP);
    sleep(1);
    BP.set_motor_position_relative(motor_middle, turn*-1-10);           // Turn the motor back to original position
    sleep(1);
}

bool lookForward(sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP){
    usleep(1000*400);    
    getUltraSValue(PORT_4, UltraSonic, BP);                             //Get value of USsensor and put it in a struct
    float afstand_in_centimeter = UltraSonic.cm;                        //Get the last struct value
    cout << "Waarde is: " << afstand_in_centimeter << endl;             //Print the last struct value (last measurement)
}

void whereToLook(vector<vector<char>> & grid, const char & look_direction, const char & facing_direction,
        vector<size_t> position, sensor_ultrasonic_t & UltraSonic, BrickPi3 & BP){
    /* Function to look for a barrier in the way of the fastest direction */
    switch (facing_direction){
        case 'D':
            // Assuming the robot is looking down
            if(look_direction == 'F') {
                // Looks forward if look direction is forward
                lookForward(UltraSonic, BP);
                if (UltraSonic.cm < 30) {
                    updateBarrier(grid, position[0], position[1] + 2);  // Update barrier if it has detected an object
                    printGrid(grid);
                    lookLeft(UltraSonic, BP);                           // Looks to the left if an object is detected down
                    if (UltraSonic.cm < 20) {
                        updateBarrier(grid, position[0] + 2, position[1]);
                    }
                }
            }else if(look_direction == 'L') {
                // Looks to the left if robot needs to look left
                lookLeft(UltraSonic, BP);
                if (UltraSonic.cm < 20) {
                    updateBarrier(grid, position[0] + 2, position[1]);  // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'R'){
                // Looks to the right if robot needs to look right
                lookRight(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]-2, position[1]);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }
            break;
        case 'R':
            // Assuming the robot is looking to the right
            if(look_direction == 'R'){
                // Looks to the right if robot needs to look right
                lookRight(UltraSonic, BP);
                if(UltraSonic.cm < 30){
                    updateBarrier(grid, position[0], position[1]+2);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
                lookForward(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]+2, position[1]);    // Updates barrier if object has been detected
                }
            }else if(look_direction == 'F'){
                // Looks forward if look direction is forward
                lookForward(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]+2, position[1]);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'L'){
                // Looks to the left if robot needs to look left
                lookLeft(UltraSonic, BP);
                if(UltraSonic.cm < 30){
                    updateBarrier(grid, position[0], position[1]-2);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }
	    break;
        case 'L':
            // Assuming the robot is looking to the left
            if(look_direction == 'L'){
                // Looks to the left if robot needs to look left
                lookLeft(UltraSonic, BP);
                if(UltraSonic.cm < 30){
                    updateBarrier(grid, position[0], position[1]+2);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'F'){
                // Looks forward if look direction is forward
                lookForward(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]-2, position[1]);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'R'){
                // Looks to the right if robot needs to look right
                lookRight(UltraSonic, BP);
                if(UltraSonic.cm < 30){
                    updateBarrier(grid, position[0], position[1]-2);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }
	        break;
        case 'U':
            // Assuming the robot is looking to the right
            if(look_direction == 'L'){
                // Looks left if look direction is left
                lookLeft(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]-2, position[1]);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'F'){
                // Looks forward if look direction is forward
                lookForward(UltraSonic, BP);
                if(UltraSonic.cm < 30){
                    updateBarrier(grid, position[0], position[1]+2);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }else if(look_direction == 'R'){
                // Looks to the right if robot needs to look right
                lookRight(UltraSonic, BP);
                if(UltraSonic.cm < 20){
                    updateBarrier(grid, position[0]+2, position[1]);    // Updates barrier if object has been detected
                    printGrid(grid);
                }
            }
            break;
    }
}

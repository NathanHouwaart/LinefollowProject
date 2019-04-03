//
// Created by Nathan Houwaart on 2019-04-03.
//

#include "linefollower.h"

using namespace std;

uint64_t factorial(uint64_t n){
    /* Recursive function to calculate the factorial of n */
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

unsigned int possibleRoutes(const unsigned int & grid_height, const unsigned int & grid_width){
    /* The amount of possible routes is calculated using the combination calculation.
     * The formula for a combination of choosing r !unique! ways from n possibilities is:
     * nCr = ( n! ) / ( r! (n - r)!) */

    unsigned int total_routes;
    uint64_t n = grid_height + grid_width;
    uint64_t r = grid_height;
    uint64_t n_factorial = factorial(n);
    uint64_t r_factorial = factorial(r);
    uint64_t n_min_r_factorial = factorial(n-r);

    total_routes = n_factorial / (r_factorial * n_min_r_factorial);
    return total_routes;
}

vector<char> fastestRoute(const unsigned int & grid_height, const unsigned int & grid_width, char & current_facing_position, BrickPi3 & BP){
    /* Function to define the fastest route on a grid. In our case this is (assuming the robot is placed in the
     * upper right position of the grid and facing down) all the way down and then all the way to the right.
     * function returns a vector<char<> with robot instructions. */
    if(current_facing_position == 'R'){
        driveOnSpot('R',BP);

    }else if(current_facing_position == 'U'){
        driveOnSpot('L',BP);
        driveOnSpot('L',BP);
    }else if(current_facing_position == 'L'){
        driveOnSpot('L',BP);
    }
    current_facing_position = 'D';
    vector<char> fastest_route = {};                            // Initialise data struct
    for(unsigned int i = 0; i < grid_height; i++){
        fastest_route.push_back('F');                           // Go all the way down on the grid
    }
    fastest_route.push_back('L');
    for(unsigned int i = 1; i < grid_width; i++){
        fastest_route.push_back('F');                           // If robot is all the way down, go all the way to the right
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
     * */

    vector<vector<char>> grid = {};                                 // Construct the grid vector

    for(int i = 0; i < (grid_height*2)+1; i++) {                    // Setup x-coordinates constructor
        vector<char> linevector = {};
        for (int j = 0; j < (grid_width*2)+1; j++) {                // Setup y-coordinates constructor
            if(i == 0 && j == 0){                                   // Set starting position of the robot
                cout << 'R';
                linevector.push_back('R');
            }else if(i%2 != 1 && j%2 != 1){                         // If i and j are even: construct a X
                cout << 'X';
                linevector.push_back('X');
            }else if(i%2 != 1 && j%2){                              // If i is even and j is uneven: construct a -
                cout << " - ";
                linevector.push_back('-');
            }else if(i%2 && j%2 != 1){                              // If i is uneven and j is even: construct a |
                cout << "|  ";
                linevector.push_back('|');
            }else{                                                  // Else construct a white space
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

char updateRobotPosition(vector<vector<char>> & grid, const char & driven_robot_direction, const char & current_facing_direction) {
    /* Updates the grid vector accordingly to the movement of the robot. Expects three parameters:
     * 1. The grid
     * 2. The direction the robot has driven
     * 3. the current facing direction of the robot. */

    size_t x_coordinate = 0, y_coordinate = 0;
    char new_facing_direction = 'D';

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 'R') {
                x_coordinate = j;   // x coordinate of R
                y_coordinate = i;   // y coordinate of R
            }
        }
    }

    switch (current_facing_direction) {
        case 'D':                                   // Facing down
            if (driven_robot_direction == 'F') {          // If the robot has driven forward, it went down
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate + 2][x_coordinate]= 'R';
                new_facing_direction = 'D';
            } else if (driven_robot_direction == 'B') {   // if the robot has driven backwards, it went up, turning in direction to up
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate - 2][x_coordinate ] = 'R';
                new_facing_direction = 'U';
            } else if (driven_robot_direction == 'R') {   // If the robot has driven to the right, it went right. turning in direction to the right
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate][x_coordinate + 2] = 'R';
                new_facing_direction = 'R';
            } else if (driven_robot_direction == 'L') {   // If the robot has driven to the left, it went left, turning in direction to the left
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate ][x_coordinate - 2] = 'R';
                new_facing_direction = 'L';
            }
            break;
        case 'U': // Facing up
            if (driven_robot_direction == 'F') {          // If the robot has driven forward, it went up
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate - 2][x_coordinate] = 'R';
                new_facing_direction = 'U';
            } else if (driven_robot_direction == 'B') {   // if the robot has driven backwards, it went down, turning in direction to down
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate + 2][x_coordinate] = 'R';
                new_facing_direction = 'D';
            } else if (driven_robot_direction == 'R') {   // If the robot has driven to the right, it went right. turning in direction to the right
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate][x_coordinate + 2] = 'R';
                new_facing_direction = 'R';
            } else if (driven_robot_direction == 'L') {   // If the robot has driven to the left, it went left, turning in direction to the left
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate][x_coordinate - 2] = 'R';
                new_facing_direction = 'L';
            }
            break;
        case 'R': // Facing right
            if (driven_robot_direction == 'F') {          // If the robot has driven forward, it went right, facing right
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate][x_coordinate + 2] = 'R';
                new_facing_direction = 'R';
            } else if (driven_robot_direction == 'B') {   // if the robot has driven backwards, it went left, turning in direction to left
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate ][x_coordinate - 2] = 'R';
                new_facing_direction = 'L';
            } else if (driven_robot_direction == 'R') {   // If the robot has driven to the right, it went down. turning in direction to down
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate + 2][x_coordinate ] = 'R';
                new_facing_direction = 'D';
            } else if (driven_robot_direction == 'L') {   // If the robot has driven to the left, it went up, turning in direction to the up
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate - 2][x_coordinate ] = 'R';
                new_facing_direction = 'U';
            }
            break;
        case 'L': // Facing left
            if (driven_robot_direction == 'F') {          // If the robot has driven forward, it went right, facing left
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate ][x_coordinate - 2] = 'R';
                new_facing_direction = 'L';
            } else if (driven_robot_direction == 'B') {   // if the robot has driven backwards, it went right, turning in direction to right
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate ][x_coordinate + 2] = 'R';
                new_facing_direction = 'R';
            } else if (driven_robot_direction == 'R') {   // If the robot has driven to the right, it went up. turning in direction to up
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate - 2][x_coordinate ] = 'R';
                new_facing_direction = 'U';
            } else if (driven_robot_direction == 'L') {   // If the robot has driven to the left, it went down, turning in direction to the down
                grid[y_coordinate][x_coordinate] = 'X';
                grid[y_coordinate + 2][x_coordinate] = 'R';
                new_facing_direction = 'L';
            }
            break;
    }
    return new_facing_direction;
}

void updateBarrier(vector<vector<char>> & grid, vector<int> barrier_coordinates){
    /* Function to add a barrier to a grid. This is used so the robot can calculate whether or not it can pass through a point */
    grid[barrier_coordinates[1]][barrier_coordinates[0]] = '+';
    printGrid(grid);
}
project(LinefollowProject)

add_executable(LinefollowProject Main/BrickPi3.cpp Main/main.cpp Main/sourceCrossroad.cpp Main/sourceDodgeObject.cpp Main/sourceDriveRobot.cpp
        Main/sourceFreeRideLoop.cpp Main/sourceGridFollowFuncitons.cpp Main/sourceGridFollowLoop.cpp Main/sourceLineFollowLoop.cpp Main/sourceMainFunctions.cpp
        Main/sourcePIDController.cpp Main/sourceSensors.cpp)
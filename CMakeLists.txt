project(LinefollowProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(LinefollowProject Main/BrickPi3.cpp Main/main.cpp Main/sourceCrossroad.cpp Main/sourceDodgeObject.cpp Main/sourceDriveRobot.cpp Main/sourceFreeRideLoop.cpp
        Main/sourceGridFollowLoop.cpp Main/sourceGridFollowFuncitons.cpp Main/sourceGridObjectDetect.cpp Main/sourceLineFollowLoop.cpp Main/sourceMainFunctions.cpp
        Main/sourcePIDController.cpp Main/sourceSensors.cpp Main/sourceSound.cpp Main/sourceLcd.cpp Main/sourcePController.cpp)

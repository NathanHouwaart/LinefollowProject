CXX=g++
OBJ = BrickPi3.o main.o sourceCrossroad.o sourceDodgeObject.o sourceDriveRobot.o sourceFreeRideLoop.o sourceGridFollowFuncitons.o sourceGridFollowLoop.o sourceLineFollowLoop.o sourceMainFunctions.o sourcePIDController.o sourceSensors.o sourceSound.o sourceLcd.o -lwiringPi

testbuild: $(OBJ)
	$(CXX) -o $@ $^

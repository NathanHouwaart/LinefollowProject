CXX=g++
OBJ = BrickPi3.o main.o sourceCrossroad.o sourceDodgeObject.o sourceDriveRobot.o sourceFreeRideLoop.o sourceGridFollowFuncitons.o sourceGridFollowLoop.o sourceLineFollowLoop.o sourceMainFunctions.o sourcePIDController.o sourceSensors.o sourceSound.o BluetoothSocket.o sourceLcd.o sourcePController.o

testbuild: $(OBJ)
	$(CXX) -o $@ $^ -lbluetooth -pthread -lwiringPi

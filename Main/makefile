CXX=g++
OBJ = BrickPi3.o main.o sourceCrossroad.o sourceDodgeObject.o sourceDriveRobot.o sourceFreeRideLoop.o sourceGridFollowFuncitons.o sourceGridFollowLoop.o sourceGridFollowLoopPID.o sourceLcd.o sourceLineFollowController.o sourceLineFollowLoop.o sourceLineFollowLoopPID.o sourceMainFunctions.o sourcePIDController.o sourceSensors.o sourceSound.o BluetoothSocket.o

testbuild: $(OBJ)
	$(CXX) -o $@ $^ -lbluetooth -pthread -lwiringPi

#include "linefollower.h"

using namespace std;

// TODO: --> Maak loop van de modus freeRideLoop() (Zie activity diagram)

void freeRideLoop(BrickPi3 & BP){

    BluetoothServerSocket serversock(2, 1);  //2 is het channel-number
    cout << "listening" << endl;
    while(true) {
        BluetoothSocket* clientsock = serversock.accept();
        cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
        MessageBox& mb = clientsock->getMessageBox();

        float steer = 1;
        string input;
        while(mb.isRunning()) {
            input = mb.readMessage();  //blokkeert niet
            if(input != "") {
                cout << endl << input << endl;
                selectDirection(input, BP, steer);
            }
            cout << ".";
            cout.flush();
            usleep(200000); // wacht 500 ms
        }
        clientsock->close();
    }
}

void selectDirection(const string & input, BrickPi3 & BP, float & steer) {

    if (input == "UP") {
        steer = 1;
        drive(1, 100, 360, BP);
    } else if (input == "DOWN") {
        steer = 1;
        drive(-2, 100, 360, BP);
    } else if (input == "FIRE") {
        drive(-1, 100, 360, BP);
    } else if (input == "LEFT") {
        steer -= 0.2;
        if(steer <= 0){
            steer = 0;
        }
        drive(steer, 100, 360, BP);
    } else if (input == "RIGHT") {
        steer += 0.2;
        if(steer <= 2){
            steer = 2;
        }
        drive(steer, 100, 360, BP);
    } else {
        int playing = 0;
        if (input == "A") {
            playSound('T', playing);
        }else if (input == "B"){
            playSound('X', playing);
        }else if (input == "C"){
            playSound('C', playing);
        }
    }
}
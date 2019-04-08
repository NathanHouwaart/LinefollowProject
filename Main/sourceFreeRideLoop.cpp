#include "linefollower.h"

using namespace std;

// TODO: --> Maak loop van de modus freeRideLoop() (Zie activity diagram)

void freeRideLoop(BrickPi3 & BP){
    int playing = 0;
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
                selectDirection(input, BP, steer, playing);
            }
            cout << ".";
            cout.flush();
            usleep(200000); // wacht 500 ms
        }
        clientsock->close();
    }
}

void selectDirection(const string & input, BrickPi3 & BP, float & steer, int & playing) {

    if (input == "UP") {
        playSound('F', playing);
        steer = 1;
        drive(1, 100, 500, BP);
    } else if (input == "DOWN") {
        playSound('B', playing);
        steer = 1;
        drive(-2, 100, 500, BP);
    } else if (input == "FIRE") {
        drive(-1, 100, 500, BP);
    } else if (input == "LEFT") {
        playSound('F', playing);
        steer -= 0.2;
        if(steer <= 0){
            steer = 0;
        }
        drive(steer, 100, 500, BP);
    } else if (input == "RIGHT") {
        playSound('F', playing);
        steer += 0.2;
        if(steer >= 2){
            steer = 2;
        }
        drive(steer, 100, 500, BP);
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

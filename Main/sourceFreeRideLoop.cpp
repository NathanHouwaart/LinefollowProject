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
                selectDirection(input, BP);
            }
            cout << ".";
            cout.flush();
            usleep(500000); // wacht 500 ms
        }

        clientsock->close();


    }
}
}

selectDirection(const string & input, BrickPi3 & BP, float & steer) {
    if (input == "UP") {
        drive(1, 100, 360, BP);
    } else if (input == 'DOWN') {
        drive(-2, 100, 360, BP);
    } else if (input == 'FIRE') {
        drive(-1, 100, 360, BP);
    } else if (input == 'LEFT') {
        steer -= 0.1;
        drive(steer, 100, 360, BP);
    } else if (input == 'RIGHT') {
        steer += 0.1;
        drive(steer, 100, 360, BP);
    }
}
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

        string input;
        while(mb.isRunning()) {
            input = mb.readMessage();  //blokkeert niet
            if(input != "") {
                cout << endl << input << endl;
                switch (input){
                    case 'RIGHT':
                        cout << 'GOING RIGHT' << endl;
                        break;
                    case 'UP':
                        cout << 'GOING UP' << endl;
                        break;
                    case 'LEFT':
                        cout << 'GOING LEFT' << endl;
                        break;
                    case 'DOWN':
                        cout << 'GOING DOWN' << endl;
                        break;
                    case 'A':
                        cout << 'GOING A' << endl;
                        break;
                    case 'B':
                        cout << 'GOING B' << endl;
                        break;
                    case 'C':
                        cout << 'GOING C' << endl;
                        break;
            }

            cout << ".";
            cout.flush();
            usleep(500000); // wacht 500 ms
        }

        clientsock->close();

    }
}
}

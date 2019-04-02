#include <iostream>
using namespace std;

int main() {
    cout << "Play" << endl;
    system("omxplayer -o local Darude\ -\ Sandstorm.mp3");
    for (int i=0; i<1000; i++) {
        cout << i << endl;
    }
    system("q");
}

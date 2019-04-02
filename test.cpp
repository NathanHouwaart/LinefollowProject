#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    cout << "Play" << endl;
    system("omxplayer --no-keys -o local Sandstorm.mp3 &");
    sleep(1);
    system("echo 'hoi'");
//    for (int i=0; i<100000; i++) {
//        cout << i << endl;
//    }
    cout << "hooi" << endl;
    system("fg");
    system("2");
    sleep(5);
    cout << "lol" << endl;
    system("killall omxplayer.bin");
}

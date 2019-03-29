#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

int pin = 4;

int main() {
	if (wiringPiSetup() == -1) {
		cout << "nope" << endl;
		return 1;
	}
	pinMode(4, OUTPUT);
	while (true) {
		cout << "aan" << endl;
		digitalWrite(4, 1);
		sleep(1);
		cout << "uit" << endl;
		digitalWrite(4, 0);
		sleep(1);
	}
	return 0;
}

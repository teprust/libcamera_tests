#include <iostream>
#include <cstdlib>
#include <wiringPi.h>

using namespace std;

void capture(int cam) {
    string cmd = "libcamera-still -o capture_" + to_string(cam) + ".jpg";
    system(cmd.c_str());
}

void mainLoop() {
    cout << "Start testing the camera A" << endl;
    system("i2cset -y 1 0x70 0x00 0x04");
    digitalWrite(7, LOW);
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    capture(1);
    
    cout << "Start testing the camera B" << endl;
    system("i2cset -y 1 0x70 0x00 0x05");
    digitalWrite(7, HIGH);
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    capture(2);
    
    cout << "Start testing the camera C" << endl;
    system("i2cset -y 1 0x70 0x00 0x06");
    digitalWrite(7, LOW);
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    capture(3);
    
    cout << "Start testing the camera D" << endl;
    system("i2cset -y 1 0x70 0x00 0x07");
    digitalWrite(7, HIGH);
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    capture(4);
}

int main() {
    wiringPiSetup();
    pinMode(7, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    
    mainLoop();
    
    digitalWrite(7, LOW);
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    
    return 0;
}

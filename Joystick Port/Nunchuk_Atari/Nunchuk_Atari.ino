#include <NintendoExtensionCtrl.h>
Nunchuk nchuk;

#define trigger       2
#define button_up     3
#define button_down   4
#define button_left   5
#define button_right  6

void setup() {
    Serial.begin(115200);

  pinMode(trigger, OUTPUT);
  pinMode(button_up, OUTPUT);
  pinMode(button_down, OUTPUT);
  pinMode(button_left, OUTPUT);
  pinMode(button_right, OUTPUT);

  digitalWrite(trigger, LOW);
  digitalWrite(button_up, LOW);
  digitalWrite(button_down, LOW);
  digitalWrite(button_left, LOW);
  digitalWrite(button_right, LOW);

    nchuk.begin();

    while (!nchuk.connect()) {
        Serial.println("Nunchuk not detected!");
        delay(1000);
    }
}

void loop() {
    boolean success = nchuk.update();

    if (success == true) {
    boolean buttonZ = nchuk.buttonZ();
    int joyY = nchuk.joyY();
    int joyX = nchuk.joyX();

    digitalWrite(trigger, buttonZ);
    digitalWrite(button_up, joyY == 255);
    digitalWrite(button_down, joyY == 0);
    digitalWrite(button_left, joyX == 0);
    digitalWrite(button_right, joyX == 255);
    }
    else {  // Data is bad :(
        Serial.println("Controller Disconnected!");
        delay(1000);
        nchuk.connect();
    }
}

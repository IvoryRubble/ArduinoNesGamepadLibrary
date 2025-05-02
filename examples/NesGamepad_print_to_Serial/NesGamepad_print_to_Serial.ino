// Read buttons from gamepad and print buttons states to serial port

#include <NesGamepad.h>

const int latchPin = A0;
const int pulsePin = A1;
const int dataPin = A2;
NesGamepad gamepad(latchPin, pulsePin, dataPin);

const char* btnNames[gamepad.btnsCount] = {
  "A",
  "B",
  "Select",
  "Start",
  "Up",
  "Down",
  "Left",
  "Right"
};

void setup() {
  Serial.begin(115200);
  gamepad.init();
}

void loop() {
  gamepad.update();

  bool btns[gamepad.btnsCount];
  btns[0] = gamepad.btnA;
  btns[1] = gamepad.btnB;
  btns[2] = gamepad.btnSelect;
  btns[3] = gamepad.btnStart;
  btns[4] = gamepad.btnUp;
  btns[5] = gamepad.btnDown;
  btns[6] = gamepad.btnLeft;
  btns[7] = gamepad.btnRight;

  String s = String();
  for (int i = 0; i < gamepad.btnsCount; i++) {
    s = s + btnNames[i] + ":" + btns[i] + " ";
  }
  Serial.println(s);

  delay(500);
}

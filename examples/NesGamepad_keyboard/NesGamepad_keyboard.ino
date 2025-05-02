// Press Start on gamepad during startup to enable serial output 

#include <Keyboard.h>
#include <NesGamepad.h>
#include "ButtonDebounce.h"

bool serialPrintEnabled = false;
unsigned long previousBtnUpdateTime = 0;

const int latchPin = A0;
const int pulsePin = A1;
const int dataPin = A2;
const unsigned int delayBeforeReadMicros = 6;
NesGamepad gamepad(latchPin, pulsePin, dataPin, delayBeforeReadMicros);

unsigned long debounceDelay = 25;
ButtonDebounce btnDebouces[gamepad.btnsCount] = {
  {debounceDelay},
  {debounceDelay},
  {debounceDelay},
  {debounceDelay},
  {debounceDelay},
  {debounceDelay},
  {debounceDelay},
  {debounceDelay}
};

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

const uint8_t keysKeyboard[gamepad.btnsCount] = {
  'k',
  'j',
  '\\',
  KEY_RETURN,
  'w',
  's',
  'a',
  'd'
};

void setup() {
  gamepad.init();

  delay(2000);
  int gamepadReadigsToDiscard = 2;
  for (int i = 0; i < gamepadReadigsToDiscard + 1; i++) {
    gamepad.update();
  }

  initSerialPrintEnableFlag();

  Keyboard.begin();
}

void loop() {
  gamepad.update();
  btnDebouces[0].updateState(gamepad.btnA);
  btnDebouces[1].updateState(gamepad.btnB);
  btnDebouces[2].updateState(gamepad.btnSelect);
  btnDebouces[3].updateState(gamepad.btnStart);
  btnDebouces[4].updateState(gamepad.btnUp);
  btnDebouces[5].updateState(gamepad.btnDown);
  btnDebouces[6].updateState(gamepad.btnLeft);
  btnDebouces[7].updateState(gamepad.btnRight);

  updateKeyboard();

  if (serialPrintEnabled) {
    printGamepadStatus();
  }
}

void initSerialPrintEnableFlag() {
  if (gamepad.btnStart) {
    serialPrintEnabled = true;
    Serial.begin(115200);
    delay(5000);
    Serial.println();
    Serial.println("Please stand by...");
    delay(1000);
    Serial.println();
    Serial.println("Enabled serial output by pressing Start on gamepad during startup");
  } else {
    serialPrintEnabled = false;
  }
}

void updateKeyboard() {
  for (int i = 0; i < gamepad.btnsCount; i++) {
    if (btnDebouces[i].isBtnPressed) {
      Keyboard.press(keysKeyboard[i]);
    }
    if (btnDebouces[i].isBtnReleased) {
      Keyboard.release(keysKeyboard[i]);
    }
  }
}

void printGamepadStatus() {
  unsigned long currentTime = millis();
  unsigned long longDelayTimeout = 1000;
  for (int i = 0; i < gamepad.btnsCount; i++) {
    if (btnDebouces[i].isBtnPressed) {
      if (currentTime - previousBtnUpdateTime > longDelayTimeout) Serial.println();
      Serial.print("+ "); Serial.print(currentTime - previousBtnUpdateTime); Serial.print(" ms "); Serial.print(btnNames[i]); Serial.println(" pressed");
      previousBtnUpdateTime = currentTime;
    }
    if (btnDebouces[i].isBtnReleased) {
      if (currentTime - previousBtnUpdateTime > longDelayTimeout) Serial.println();
      Serial.print("+ "); Serial.print(currentTime - previousBtnUpdateTime); Serial.print(" ms "); Serial.print(btnNames[i]); Serial.println(" released");
      previousBtnUpdateTime = currentTime;
    }
  }
}

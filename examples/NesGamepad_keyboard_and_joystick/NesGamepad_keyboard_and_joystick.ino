// Press Start on gamepad during startup to enable serial output  
// Press Start+A on gamepad during startup to change output mode to keyboard
// Press Start+B on gamepad during startup to change output mode to joystick

#include <Keyboard.h>
// Install Joystick lib from here: https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Joystick.h>

#include <EEPROM.h>
#include <NesGamepad.h>
#include "ButtonDebounce.h"

bool serialPrintEnabled = false;
unsigned long previousBtnUpdateTime = 0;

const int outputModesCount = 2;
enum OutputMode {
  keyboardOutputMode = 0,
  joystickOutputMode = 1
};
const char* outputModeNames[outputModesCount] = { "keyboard", "joystick" };

OutputMode outputMode = keyboardOutputMode;
int outputModeStorageAddress = 24;

const int latchPin = A0;
const int pulsePin = A1;
const int dataPin = A2;
const unsigned int delayBeforeReadMicros = 6;
NesGamepad gamepad(latchPin, pulsePin, dataPin, delayBeforeReadMicros);

Joystick_ joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 4, 1, false, false, false, false, false, false, false, false, false, false, false);

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

enum ButtonIndex {
  btnUpIndex = 4,
  btnDownIndex = 5,
  btnLeftIndex = 6,
  btnRightIndex = 7
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

const uint8_t keysJoystick[gamepad.btnsCount] = {
  0,
  1,
  2,
  3,
  0,
  0,
  0,
  0
};

void setup() {
  gamepad.init();

  delay(2000);
  int gamepadReadigsToDiscard = 2;
  for (int i = 0; i < gamepadReadigsToDiscard + 1; i++) {
    gamepad.update();
  }

  initSerialPrintEnableFlag();
  initOutputMode();

  if (serialPrintEnabled) {
    printOutputModeInfo();
  }

  switch (outputMode) {
    case OutputMode::keyboardOutputMode:
      Keyboard.begin();
      break;
    case OutputMode::joystickOutputMode:
      joystick.begin();
      break;
  }
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

  switch (outputMode) {
    case OutputMode::keyboardOutputMode:
      updateKeyboard();
      break;
    case OutputMode::joystickOutputMode:
      updateJoystick();
      break;  
  }

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

void initOutputMode() {
  if (gamepad.btnStart && (gamepad.btnA || gamepad.btnB)) {
    if (gamepad.btnA) outputMode = OutputMode::keyboardOutputMode;
    if (gamepad.btnB) outputMode = OutputMode::joystickOutputMode;
    EEPROM.put(outputModeStorageAddress, outputMode);
  } else {
    EEPROM.get(outputModeStorageAddress, outputMode);
    outputMode = (OutputMode)(abs(outputMode) % outputModesCount);
  }
}

void printOutputModeInfo() {
  Serial.println("Press Start+A on gamepad during startup to change output mode to keyboard");
  Serial.println("Press Start+B on gamepad during startup to change output mode to joystick");
  Serial.print("Current output mode: ");
  Serial.println(outputModeNames[outputMode]);
  Serial.println();
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

void updateJoystick() {
  for (int i = 0; i < 4; i++) {
    if (btnDebouces[i].isBtnPressed) {
      joystick.pressButton(keysJoystick[i]);
    }
    if (btnDebouces[i].isBtnReleased) {
      joystick.releaseButton(keysJoystick[i]);
    }
  }

  bool isArrowChanged = false;
  for (int i = 4; i < gamepad.btnsCount; i++) {
    isArrowChanged = isArrowChanged || (btnDebouces[i].isBtnPressed || btnDebouces[i].isBtnReleased);
  }
  if (isArrowChanged) {
    if (btnDebouces[btnUpIndex].btnState && btnDebouces[btnRightIndex].btnState) {
      joystick.setHatSwitch(0, 45);
    } else if (btnDebouces[btnRightIndex].btnState && btnDebouces[btnDownIndex].btnState) {
      joystick.setHatSwitch(0, 135);
    } else if (btnDebouces[btnDownIndex].btnState && btnDebouces[btnLeftIndex].btnState) {
      joystick.setHatSwitch(0, 225);
    } else if (btnDebouces[btnLeftIndex].btnState && btnDebouces[btnUpIndex].btnState) {
      joystick.setHatSwitch(0, 315);
    } else if (btnDebouces[btnUpIndex].btnState) {
      joystick.setHatSwitch(0, 0);
    } else if (btnDebouces[btnRightIndex].btnState) {
      joystick.setHatSwitch(0, 90);
    } else if (btnDebouces[btnDownIndex].btnState) {
      joystick.setHatSwitch(0, 180);
    } else if (btnDebouces[btnLeftIndex].btnState) {
      joystick.setHatSwitch(0, 270);
    } else {
      joystick.setHatSwitch(0, -1);
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

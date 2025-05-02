//
// NesGamepad.cpp
//
// Author:
//       Yaroslav Metelev <yaroslav.metelev@outlook.com>
//
// Copyright (c) 2025 Yaroslav Metelev <https://github.com/IvoryRubble>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <NesGamepad.h>

NesGamepad::NesGamepad(int latchPin, int pulsePin, int dataPin, unsigned int delayBeforeReadMicros) : 
  latchPin(latchPin), 
  pulsePin(pulsePin),
  dataPin(dataPin),
  delayBeforeReadMicros(delayBeforeReadMicros) {}

void NesGamepad::init() {
  pinMode(latchPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);
  digitalWrite(latchPin, LOW);
  digitalWrite(pulsePin, LOW);

  pinMode(dataPin, INPUT_PULLUP);
}

void NesGamepad::update() {
  bool btns[8]; 

  digitalWrite(pulsePin, LOW);
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(delayBeforeReadMicros * 2);
  digitalWrite(latchPin, LOW);
  delayMicroseconds(delayBeforeReadMicros);

  for (int i = 0; i < btnsCount; i++) {
    btns[i] = !digitalRead(dataPin);
    digitalWrite(pulsePin, HIGH);
    delayMicroseconds(delayBeforeReadMicros);
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(delayBeforeReadMicros);
  }

  delayMicroseconds(delayBeforeReadMicros);

  btnA = btns[0];
  btnB = btns[1];
  btnSelect = btns[2];
  btnStart = btns[3];
  btnUp = btns[4];
  btnDown = btns[5];
  btnLeft = btns[6];
  btnRight = btns[7];
}

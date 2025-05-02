// No actually a NesGamepad library example but juat reading buttons from NES gamepad and print to serial port
// also blink led

const int latchPin = A0;
const int pulsePin = A1;
const int dataPin = A2;

const unsigned int delayTimeMicroseconds = 6;

const int btnsCount = 8;
bool btns[8]; 

const char* btnNames[8] = {
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

  pinMode(latchPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);
  digitalWrite(latchPin, LOW);
  digitalWrite(pulsePin, LOW);

  pinMode(dataPin, INPUT_PULLUP);
}

void loop() {
  digitalWrite(pulsePin, LOW);
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(delayTimeMicroseconds * 2);
  digitalWrite(latchPin, LOW);
  delayMicroseconds(delayTimeMicroseconds);

  for (int i = 0; i < btnsCount; i++) {
    btns[i] = !digitalRead(dataPin);
    digitalWrite(pulsePin, HIGH);
    delayMicroseconds(delayTimeMicroseconds);
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(delayTimeMicroseconds);
  }

  String s = String();
  for (int i = 0; i < btnsCount; i++) {
    s = s + btnNames[i] + ":" + btns[i] + " ";
  }
  Serial.println(s);

  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
}

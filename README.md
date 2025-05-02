# NesGamepad  

Yet another library to read buttons from NES (aka Famicom, aka Dendy) gamepad.  

## Sources

Protocol description:  
https://tresi.github.io/nes/   
https://web.archive.org/web/20150829043041/https://www.mit.edu/~tarvizo/nes-controller.html  

Protocol reverse engineering:  
https://www.raspberryfield.life/2018/09/01/nespi-project-part-4-the-nes-controller-protocol/  

Pinout:   
https://smiheys.narod.ru/dendy.html  
https://habr.com/ru/articles/147356/  
https://habr.com/ru/articles/191936/  

## Pinout

There are several versions of NES gamepad with different connectors.      

**Original NES** "D-cannon" connector  
```
          +----> +5V (white)
          |
5 +---------+  7
  | o  o  o   \
  | o  o  o  o |
4 +------------+ 1
    |  |  |  |
    |  |  |  +-> GND (brown)
    |  |  +----> Pulse (red)
    |  +-------> Latch (orange)
    +----------> Data (yellow)
```

**NES clone** female DE-9 connector  
```
       +--------- Pulse        
       | +------- Latch
       | | +----- Data
       | | |           
5 +-------------+ 1             
   \ O O O O O /                
    \ O O O O /                  
   9 +-------+ 6                 
        |   |
        |   +---- +5V
        +-------- GND
```

**NES clone** female DA-15 connector  
```
     +------------------- Data
     |             +----- GND
     |             |           
8 +-------------------+ 1             
   \ O O O O O O O O /                
    \ O O O O O O O /                  
  15 +-------------+ 9                 
      |     |     |
      |     |     +------ Pulse
      |     +------------ Latch
      +------------------ +5V
```

## Connection

Connect `+5V` and `GND` pins to `VCC` and `GND` pins of microcontroller board.  
Connect `Latch`, `Pulse`, `Data` pins to microcontroller I/O pins.  

## Usage

Set Latch, Pulse, Data pins in `NesGamepad::NesGamepad()` constructor. Optionally set custom `delayBeforeReadMicros` delay (default is 6 microseconds).   
Example:  
```C++
const int latchPin = A0;
const int pulsePin = A1;
const int dataPin = A2;
const unsigned int delayBeforeReadMicros = 6;
NesGamepad gamepad(latchPin, pulsePin, dataPin, delayBeforeReadMicros);
```

Call `NesGamepad::init()` method in `setup()` procedure to initialize microcontroller ports for gamepad.   
Example:   
```C++
gamepad.init();
```

Call `NesGamepad::update()` method to read new values from controller.  
Then you can get buttons values from `gamepad` object fields. `True` value means corresponding button is pressed.   
Example:   
```C++
gamepad.update();
Serial.println(gamepad.btnA);
Serial.println(gamepad.btnStart);
Serial.println(gamepad.btnUp);
```


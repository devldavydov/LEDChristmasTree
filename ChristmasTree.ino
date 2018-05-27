#include <EEPROM.h>

#define COUNT_OF(array) (sizeof(array) / sizeof(array[0]))

//Pin connected to ST_CP of 74HC595
int latchPin = 7;
//Pin connected to SH_CP of 74HC595
int clockPin = 5;
//Pin connected to DS of 74HC595
int dataPin = 3;
// LED pin
int ledPin = 13;

int CLEAR[][3] = {{0, 0, 0}};

int LIGHTS_DOWN_UP[][3] = {
  {3, 0, 0}, {12, 0, 0}, {48, 0, 0}, {192, 0, 0}, {0, 0, 3}, {0, 3, 0}, {0, 12, 0}, {0, 48, 0}, {0, 192, 0}, {0, 0, 12}, {0, 0, 48},
  {0, 0, 192}, {0, 0, 48}, {0, 0, 12}, {0, 192, 0}, {0, 48, 0}, {0, 12, 0}, {0, 3, 0}, {0, 0, 3}, {192, 0, 0}, {48, 0, 0}, {12, 0, 0}
};

int LIGHTS_ROUND[][3] = {
  {2, 0, 0}, {8, 0, 0}, {32, 0 , 0}, {128, 0, 0}, {0, 0, 2}, {0, 2, 0}, {0, 8, 0}, {0, 32, 0}, {0, 128, 0}, {0, 0, 8}, {0, 0, 32}, {0, 0, 128},
  {0, 0, 64}, {0, 0, 16}, {0, 0, 4}, {0, 64, 0}, {0, 16, 0}, {0, 4, 0}, {0, 1, 0}, {0, 0, 1}, {64, 0, 0}, {16, 0, 0}, {4, 0, 0}, {1, 0, 0},
  {4, 0, 0}, {16, 0, 0}, {64, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 4, 0}, {0, 16, 0}, {0, 64, 0}, {0, 0, 4}, {0, 0, 16}, {0, 0, 64},
  {0, 0, 128}, {0, 0, 32}, {0, 0, 8}, {0, 128, 0}, {0, 32, 0}, {0, 8, 0}, {0, 2, 0}, {0, 0, 2}, {128, 0, 0}, {32, 0 , 0}, {8, 0, 0}
};

int REGISTERS_FULL[][3] = {
  {255, 0, 0}, {0, 255, 0}, {0, 0, 255}
};

int REGISTERS_CNT[][3] = {
  {1, 0, 0}, {2, 0, 0}, {4, 0, 0}, {8, 0, 0}, {16, 0, 0}, {32, 0, 0}, {64, 0, 0}, {128, 0, 0},
  {0, 1, 0}, {0, 2, 0}, {0, 4, 0}, {0, 8, 0}, {0, 16, 0}, {0, 32, 0}, {0, 64, 0}, {0, 128, 0},
  {0, 0, 1}, {0, 0, 2}, {0, 0, 4}, {0, 0, 8}, {0, 0, 16}, {0, 0, 32}, {0, 0, 64}, {0, 0, 128}
};

int SNAKE[][3] = {
  {2, 0, 0}, {10, 0, 0}, {42, 0, 0}, {170, 0, 0}, {170, 0, 2}, {170, 2, 2}, {170, 10, 2}, {170, 42, 2}, {170, 170, 2}, {170, 170, 10}, {170, 170, 42}, {170, 170, 170},
  {170, 170, 234}, {170, 170, 250}, {170, 170, 254}, {170, 234, 254}, {170, 250, 254}, {170, 254, 254}, {170, 255, 254}, {170, 255, 255}, {234, 255, 255}, {254, 255, 255},
  {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {252, 252, 207}, {252, 252, 207}, {252, 252, 207}, {240, 240, 207}, {240, 240, 207}, {240, 240, 207},
  {192, 192, 15}, {192, 192, 15}, {192, 192, 15}, {0, 0, 15}, {0, 0, 15}, {0, 0, 15}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

int RANDOM[][3] = {
  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

int mode;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Set mode
  mode = EEPROM.read(0);
  if (mode == 0) {
    EEPROM.write(0, 1);
  } else {
    mode = 1;
    EEPROM.write(0, 0);
  }
}

void processArray(int codes[][3], int len, int dly) {
  for (int i = 0; i < len; i++) {
    digitalWrite(latchPin, LOW);
    
    for (int j = 0; j <= 2; j++) {
      shiftOut(dataPin, clockPin, MSBFIRST, codes[i][j]);  
    }

    digitalWrite(latchPin, HIGH);
    delay(dly);
  }  
}

void processRandom() {
  while (1) {
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 3; i++)
      shiftOut(dataPin, clockPin, MSBFIRST, randomInRange(0, 255)); 
    digitalWrite(latchPin, HIGH);
    delay(500);
  };
}

int randomInRange(int min, int max) {
  return min + (rand() % static_cast<int>(max - min + 1));
}

void loop() {
  if (mode == 1) {
    digitalWrite(ledPin, HIGH);

    processRandom();
  } else {
    digitalWrite(ledPin, LOW);

    for (int i = 0; i < 2; i++)
      processArray(LIGHTS_DOWN_UP, COUNT_OF(LIGHTS_DOWN_UP), 100);
    processArray(CLEAR, COUNT_OF(CLEAR), 100);

    for (int i = 0; i < 2; i++)
      processArray(LIGHTS_ROUND, COUNT_OF(LIGHTS_ROUND), 100);
    processArray(CLEAR, COUNT_OF(CLEAR), 100);

    for (int i = 0; i < 2; i++)
      processArray(REGISTERS_FULL, COUNT_OF(REGISTERS_FULL), 500);
    processArray(CLEAR, COUNT_OF(CLEAR), 100);
    
    processArray(REGISTERS_CNT, COUNT_OF(REGISTERS_CNT), 200);
    processArray(CLEAR, COUNT_OF(CLEAR), 100);

    for (int i = 0; i < 2; i++)
      processArray(SNAKE, COUNT_OF(SNAKE), 100);
    processArray(CLEAR, COUNT_OF(CLEAR), 100);
  }
}

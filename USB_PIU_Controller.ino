#define SERIAL_DEBUG
#define JOYSTICK_ON

//ARROWS
// 0 # 3
// # 2 #
// 1 # 4

// PADS
//   3
// 2   0
//   1

const int muxPinA = 4;
const int muxPinB = 5;
const int arrow0Pin = 6;
const int arrow1Pin = 7;
const int arrow2Pin = 8;
const int arrow3Pin = 9;
const int arrow4Pin = 10;

const int NUMBER_OF_ARROWS = 5;
const int NUMBER_OF_PADS = 4;
const int debounceInterval = 50;

bool TEMP_PADS[NUMBER_OF_ARROWS][NUMBER_OF_PADS];
bool PADS[NUMBER_OF_ARROWS][NUMBER_OF_PADS];
int PADS_CHANGE_TIME[NUMBER_OF_ARROWS][NUMBER_OF_PADS];

#ifdef JOYSTICK_ON
#include <Joystick.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                    NUMBER_OF_ARROWS, 0,     // Button Count, Hat Switch Count
                    false, false, false,   // No X, Y, or Z Axis
                    false, false, false,   // No Rx, Ry, or Rz
                    false, false,          // No rudder or throttle
                    false, false, false);  // No accelerator, brake, or steering
#endif

void setup() {   
  pinMode(arrow0Pin, INPUT);
  pinMode(arrow1Pin, INPUT);
  pinMode(arrow2Pin, INPUT);
  pinMode(arrow3Pin, INPUT);
  pinMode(arrow4Pin, INPUT);
  pinMode(muxPinA, OUTPUT);
  pinMode(muxPinB, OUTPUT);

  #ifdef JOYSTICK_ON
  Joystick.begin();
  #endif

  #ifdef SERIAL_DEBUG
  Serial.begin(19200);
  #endif
}

void loop() {
  ReadPads();

  #ifdef JOYSTICK_ON
  SetJoystickButton(0);
  SetJoystickButton(1);
  SetJoystickButton(2);
  SetJoystickButton(3);
  SetJoystickButton(4);
  #endif

  #ifdef SERIAL_DEBUG
  SerialDebug();
  #endif
}

void SetJoystickButton(int arrow)
{
  if (PADS[arrow][0] | PADS[arrow][1] | PADS[arrow][2] | PADS[arrow][3]){
    Joystick.setButton(arrow, 1);
  }
  else{
    Joystick.setButton(arrow, 0);
  }
}

void ReadPads(){
  for (int i= 0; i<4; i++){
    digitalWrite(muxPinA, i & 0x01);
    digitalWrite(muxPinB, i & 0x02);
    
    int currentMillis = millis();
    Debounce(currentMillis, 0, i, arrow0Pin);
    Debounce(currentMillis, 1, i, arrow1Pin);
    Debounce(currentMillis, 2, i, arrow2Pin);
    Debounce(currentMillis, 3, i, arrow3Pin);
    Debounce(currentMillis, 4, i, arrow4Pin);
  }
}

void Debounce(long currentMillis, int arrow, int pad, int arrowPin){
  TEMP_PADS[arrow][pad] = digitalRead(arrowPin) == LOW;
  if (TEMP_PADS[arrow][pad] != PADS[arrow][pad] 
      && abs(currentMillis - PADS_CHANGE_TIME[arrow][pad]) >= debounceInterval){
    PADS[arrow][pad] = TEMP_PADS[arrow][pad];
    PADS_CHANGE_TIME[arrow][pad] = currentMillis;
  }
}

void SerialDebug(){
  byte padValues[NUMBER_OF_ARROWS];
  for (int i= 0; i< NUMBER_OF_ARROWS; i++){
    padValues[i] =(PADS[i][0] ? 1 : 0) |
                  (PADS[i][1] ? 2 : 0) |
                  (PADS[i][2] ? 4 : 0) |
                  (PADS[i][3] ? 8 : 0);
    Serial.print(i); Serial.print(": "); Serial.print(padValues[i], HEX); Serial.print("  ");
  }

  Serial.println();

  // Serial.Print(padValues[0]);
  // Serial.Print(" ");
  // Serial.Print(padValues[3]);
  // Serial.println();

  // Serial.Print(" ");
  // Serial.Print(padValues[2]);
  // Serial.Print(" ");
  // Serial.println();

  // Serial.Print(padValues[1]);
  // Serial.Print(" ");
  // Serial.Print(padValues[4]);
  // Serial.println();
}

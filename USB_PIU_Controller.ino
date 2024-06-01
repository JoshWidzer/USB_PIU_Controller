#define SERIAL_DEBUG
//#define JOYSTICK_ON

const int muxPinA = 7;
const int muxPinB = 8;
const int arrow0Pin = 3;

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
  if (PADS[0][0] | PADS[0][1] | PADS[0][2] | PADS[0][3]){
    Joystick.setButton(0, TRUE);
  }
#endif

#ifdef SERIAL_DEBUG
  SerialDebug(PADS);
#endif
}

void ReadPads(){
  for (int i= 0; i<4; i++){
    digitalWrite(muxPinA, i & 0x01);
    digitalWrite(muxPinB, i & 0x02);
    
    TEMP_PADS[0][i] = digitalRead(arrow0Pin) == LOW;
    Debounce(millis(), i);
  }
}

void Debounce(long currentMillis, int pad){
  if (TEMP_PADS[0][pad] != PADS[0][pad] && abs(currentMillis - PADS_CHANGE_TIME[0][pad]) >= debounceInterval){
    PADS[0][pad] = TEMP_PADS[0][pad];
    PADS_CHANGE_TIME[0][pad] = currentMillis;
  }
}

void SerialDebug(bool values[5][4]){
  byte padValue =(values[0][0] ? 1 : 0) |
                 (values[0][1] ? 2 : 0) |
                 (values[0][2] ? 4 : 0) |
                 (values[0][3] ? 8 : 0);
  Serial.print("0: ");
  Serial.print(padValue, HEX);
  Serial.println();
}

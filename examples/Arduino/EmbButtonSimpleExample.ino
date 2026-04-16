#define EMBBTN_ENABLE_STEP
#define EMBBTN_FUNCTIONS_IMPLEMENTATION
#include "embButton.h"
char btn_read() {return digitalRead(2);}
embButton_t btn;

void setup() {
  embButtonMillis = millis;
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  btn.debounceTime=25;
  btn.releaseTime=250;
  btn.stepTime=250;
  btn.holdTime=500;
  btn.buttonCheck=btn_read;
  Serial.begin(9600);
}

void loop() {
  embButtonTick(&btn);
  if (btn.s.state & 0b10) //Pressed
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

  if (btn.s.state == EMBBTN_STATE_PRS)
    digitalWrite(3, 1);
  else
    digitalWrite(3, 0);

    if (btn.s.state == EMBBTN_STATE_HLD)
    digitalWrite(4, 1);
  else
    digitalWrite(4, 0);

  if (btn.s.released)
  {
    //Serial.print((btn.lastPressType)?"-":"."); // Morse code output
    Serial.print((btn.s.lastPressType)?"Hold: ":"Clicked: ");
    Serial.println(millis()-btn.s.timer); //Press time
  }

    if (btn.s.step){
    //Serial.print(" "); // Morse code output
    Serial.print("Step\n");
  }

  if (btn.s.endClicking){
    //Serial.print(" "); // Morse code output
    Serial.print("Clicks strikes end. Clicks: " + String(btn.s.clicks) + "\n");
  }
}

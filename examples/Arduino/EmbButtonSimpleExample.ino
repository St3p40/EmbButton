#include "embButton.h"
char btn_read() {return digitalRead(2);}
embButton_t btn={0};

void setup() {
  pinMode(3, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  btn.debounceTime=25;
  btn.releaseTime=250;
  btn.holdTime=500;
  btn.millisFunc=millis;
  btn.buttonCheck=btn_read;
  Serial.begin(9600);
}

void loop() {
  embButtonTick(&btn);
    if (btn.state > 0) //Pressed
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

  if (btn.state == EMB_BTN_STATE_PRESSED)
    digitalWrite(3, 1);
  else
    digitalWrite(3, 0);

    if (btn.state == EMB_BTN_STATE_HELD)
    digitalWrite(4, 1);
  else
    digitalWrite(4, 0);

  if (btn.isReleased)
  {
    //Serial.print((btn.lastPressType)?"-":"."); // Morse code output
    Serial.print((btn.lastPressType)?"Hold ":"Clicked ");
  }

  if (btn.endClicks){
   //Serial.print(" "); // Morse code output
    Serial.print("\nClicks:" + String(btn.clicks) + "\n");
  }
}

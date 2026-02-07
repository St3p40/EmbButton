#include "embButton.h"
char btn_read() {return digitalRead(2);}
embButton_t btn={0};

void setup() {
  pinMode(2, INPUT_PULLUP);
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

  if (btn.state == PRESSED)
    digitalWrite(3, 1);
  else
    digitalWrite(3, 0);

    if (btn.state == HELD)
    digitalWrite(4, 1);
  else
    digitalWrite(4, 0);

  if (btn.lastPressType>=0)
    Serial.print((btn.lastPressType)?"Hold ":"Clicked ");
  
  if (btn.endClicks)
    Serial.print("Clicks:" + String(btn.endClicks) + "\n");
}

#include "ControlPanel28.h"

unsigned long ControlPanel28::lastKillPressMs = 0;
bool ControlPanel28::killIsrEna = false;
void (*ControlPanel28::killPressFcn)() = NULL;

ControlPanel28::ControlPanel28(int8_t cs, int8_t dc, int8_t rst, 
int8_t encBtn, int8_t encA, int8_t encB, int8_t kill, int8_t beeper) 
: encBtnPin(encBtn), 
  encAPin(encA), 
  encBPin(encB), 
  killPin(kill), 
  beeperPin(beeper),
  lastEncBtnState(HIGH),
  lastEncAState(HIGH),
  lastEncBState(HIGH),
  tft(cs, dc, rst)
{
}

void ControlPanel28::init(void (*fcnPtr)())
{
  tft.init(LDO_PANEL_W, LDO_PANEL_H);

  killPressFcn = fcnPtr;
  
  pinMode(encBtnPin, INPUT_PULLUP);
  pinMode(encAPin, INPUT_PULLUP);
  pinMode(encBPin, INPUT_PULLUP);
  pinMode(killPin, INPUT_PULLUP);
  pinMode(beeperPin, OUTPUT);

  digitalWrite(beeperPin, LOW);

  lastEncAState = digitalRead(encAPin);
  lastEncBState = digitalRead(encBPin);

  attachInterrupt(digitalPinToInterrupt(killPin), checkKillBtn, FALLING);
}

bool ControlPanel28::checkEncBtn()
{
  int buttonState = digitalRead(encBtnPin);
  bool btnPressed = false;

  if (buttonState != lastEncBtnState)
  {
    delay(bounceDelayMs);
    if ( buttonState == digitalRead(encBtnPin) )
    {
      if (buttonState == LOW)
      {
        btnPressed = true;
      }
    }
  }
  lastEncBtnState = buttonState;
  return btnPressed;
}

int ControlPanel28::checkRotaryEnc()
{
  boolean encAState = digitalRead(encAPin);
  int changeVal = 0;
  
  if ((lastEncAState == HIGH) && (encAState == LOW))
  {
    if (digitalRead(encBPin) == LOW)
      changeVal = -1;
    else
      changeVal = 1;
  }
  
  lastEncAState = encAState;
  return changeVal;
}

void ControlPanel28::checkKillBtn()
{
  if (killIsrEna) 
  {
    unsigned long pressTimeMs = millis();
    if (pressTimeMs - lastKillPressMs > 500)
    {
      lastKillPressMs = pressTimeMs;
      killPressFcn();
    }
  }
  else
  {
    killIsrEna = true;
  }
}

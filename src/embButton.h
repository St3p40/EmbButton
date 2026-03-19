#ifndef __EMBBUTTON_H__
#define __EMBBUTTON_H__

//                                .::.
//                           =@@@@%**#@@@@+
//                       #@@@+            =@@@#.         @@*
//                   -@%=       -@@@@@@=       =%@-      @@*
//    +%@%+         *%#        -@@@@@@@@-        #%#     @@* +%@%*
//  @@=  -@@*       #@%           @@@@           %@%     @@@@@=  =@@
// %@@@@@@@@%.      %@@                          @@%     @@@@     +@%
// @@-              @@@                          @@@     @@@@.    :@@
// :@@@%=:@@=  =@*  %@+       @@@@@@@@@@@@       +@@     @@@@@-+@@@@-
//     --=.    =@#                +@@*                   @@# .=--
//             *@%:               -@@=      =#%%*
//             %@@@@@ #@*  +@      %%      @@. @@% %@#=
//             @@* %@ @@*  +@.            .@@  *@@ @@*@@@
//             @@@@@# %@%  %@-  ++%@@%++  :@%  #@% @@ -@@
//             .+=    .@@-.@@.     @@.    .@@:-@@.    =@@
//                      .++-       ##       -++.
//Made by st3p40
//06.02.26

#ifndef EmbVarFunPrefix
#define EmbVarFunPrefix static inline
#endif

#ifndef EmbBtnMaxClicks
#define EmbBtnMaxClicks 7
#endif

#ifndef EmbBtnMaxClicksSize
#define EmbBtnMaxClicksSize 3
#endif

#ifdef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC embButtonMillisFunc
EmbVarFunPrefix unsigned long (*embButtonMillisFunc) ();
#endif

#ifdef EmbBtnOneDebTimer
#define _EMBBTNDEBTIMER embButtonDebTimer
EmbVarFunPrefix const unsigned int embButtonDebTimer = EmbBtnOneDebTimer;
#endif

#ifdef EmbBtnOneHoldTimer
#define _EMBBTNHOLDTIMER embButtonHoldTimer
EmbVarFunPrefix const unsigned int embButtonHoldTimer = EmbBtnOneHoldTimer;
#endif

#ifdef EmbBtnOneReleaseTimer
#define _EMBBTNRELEASETIMER embButtonReleaseTimer
EmbVarFunPrefix const unsigned int embButtonReleaseTimer = EmbBtnOneReleaseTimer;
#endif

typedef enum
{
  EMB_BTN_STATE_RELEASED = -1,
  EMB_BTN_STATE_AWAIT = 0,
  EMB_BTN_STATE_PRESSED,
  EMB_BTN_STATE_HELD
} embButtonState;

typedef struct
{
  char isClicked: 1;
  char isReleased: 1;
  char isHold: 1;
  char endClicks: 1;

  char lastPressType :1;

  unsigned short clicks :EmbBtnMaxClicksSize;

  unsigned int timer;
#ifndef EmbBtnOneHoldTimer
#define _EMBBTNHOLDTIMER btn->holdTime
  unsigned int holdTime;
#endif
#ifndef EmbBtnOneReleaseTimer
#define _EMBBTNRELEASETIMER btn->releaseTime
  unsigned int releaseTime;
#endif
#ifndef EmbBtnDisableDebounce
  char _lastState;
  unsigned int _lastChange;
#ifndef EmbBtnOneDebTimer
#define _EMBBTNDEBTIMER btn->debounceTime
  unsigned int debounceTime;
#endif
#ifdef EmbBtnStepTimer
  unsigned int secondTimer;
  unsigned int stepTime;
  char step;
#endif
#endif
  embButtonState state;
#ifdef EmbBtnCheckButtonAsValue
  char *buttonCheck;
#else
  char (*buttonCheck) ();
#endif
#ifndef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC btn->millisFunc
  unsigned long (*millisFunc) ();
#endif

#ifdef EmbBtnUseActionCallbacks
  void (*clickedCallback) ();
  void (*heldCallback) ();
  void (*releasedCallback) ();
  void (*endClicksCallback) ();
#endif
} embButton_t;

#ifdef EmbButtonHandleMultipleButtons
EmbVarFunPrefix void embButtonTick(embButton_t *btns, size_t am)
{
  if (!btns)
    return;
  for (embButton_t* btn = btns; btn < btns + am; btn++){
#else
EmbVarFunPrefix void embButtonTick(embButton_t *btn)
{
  if (!btn)
    return;
#endif
  if (!btn->buttonCheck || !_EMBBTNMILLISFUNC)
  {
    return;
  }
  unsigned long t = _EMBBTNMILLISFUNC();
#ifdef EmbBtnCheckButtonAsValue
  char reading = *btn->buttonCheck;
#else
  char reading = btn->buttonCheck();
#endif
  btn->isHold = 0;
  if (btn->isClicked)
  {
    btn->timer = t;
    btn->isClicked = 0;
  }
  if (btn->isReleased)
  {
      btn->timer = t;
      btn->isReleased = 0;
  }
  if (btn->endClicks)
  {
    btn->clicks = 0;
    btn->endClicks = 0;
  }
#ifdef EmbBtnStepTimer
    btn->step = 0;
#endif

#ifndef EmbBtnDisableDebounce
  if (reading != btn->_lastState)
  {
    btn->_lastChange = t;
  }
  if (t - btn->_lastChange > _EMBBTNDEBTIMER)
  {
#endif
    if (reading)
    {
      switch (btn->state)
      {
        case EMB_BTN_STATE_AWAIT: case EMB_BTN_STATE_RELEASED:
          btn->state = EMB_BTN_STATE_PRESSED;
          btn->clicks++;
          btn->isClicked = 1;
        break;

        case EMB_BTN_STATE_PRESSED:
          if (t - btn->timer >= _EMBBTNHOLDTIMER)
          {
            btn->state = EMB_BTN_STATE_HELD;
            btn->isHold = 1;
#ifdef EmbBtnStepTimer
            btn->secondTimer = t;
#endif
          }
          break;
#ifdef EmbBtnStepTimer
        case EMB_BTN_STATE_HELD:
          if (t - btn->secondTimer >= btn->stepTime)
          {
            btn->step = 1;
            btn->secondTimer = t;
          }
          break;
#endif
      }
    }
    else
    {
      switch (btn->state)
      {
        case EMB_BTN_STATE_PRESSED: case EMB_BTN_STATE_HELD:
          btn->isReleased = 1;
          btn->lastPressType = btn->state - 1;
#ifdef EmbBtnEndClicksAfterHolding
          if (btn->clicks >= EmbBtnMaxClicks || btn->state == EMB_BTN_STATE_HELD)
          {
#else
          if (btn->clicks >= EmbBtnMaxClicks)
          {
#endif
            btn->state = EMB_BTN_STATE_AWAIT;
            btn->endClicks = 1;
          }
          else
          {
            btn->state = EMB_BTN_STATE_RELEASED;
          }
          break;

        case EMB_BTN_STATE_RELEASED:
          if (t - btn->timer >= _EMBBTNRELEASETIMER)
          {
            btn->state = EMB_BTN_STATE_AWAIT;
            btn->endClicks = 1;
          }
         break;
      }
    }
#ifndef EmbBtnDisableDebounce
  }
  btn->_lastState = reading;
#endif
#ifdef EmbButtonHandleMultipleButtons
  }
#endif
}
#ifdef EmbBtnUseActionCallbacks
#ifdef EmbButtonHandleMultipleButtons
EmbVarFunPrefix void embButtonaActionCallcback(embButton_t *btns, size_t am)
{
  if (!btns)
    return;
  for (embButton_t* btn = btns; btn < btns + am; btn++){
#else
EmbVarFunPrefix void embButtonaActionCallcback(embButton_t *btn)
{
  if (!btn)
  return;
#endif
  if (btn->isClicked)
    btn->clickedCallback();
  if (btn->isHold)
    btn->heldCallback();
  if (btn->isReleased)
    btn->releasedCallback();
  if (btn->endClicks)
    btn->endClicksCallback();
#ifdef EmbButtonHandleMultipleButtons
  }
#endif
}
#endif

#endif
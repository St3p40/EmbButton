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

#ifndef EmbBtnMaxClicks
#define EmbBtnMaxClicks 254
#endif

#ifdef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC embButtonMillisFunc
unsigned long (*embButtonMillisFunc) ();
#endif

#ifdef EmbBtnOneDebTimer
#define _EMBBTNDEBTIMER embButtonDebTimer
const unsigned int embButtonDebTimer = EmbBtnOneDebTimer;
#endif

#ifdef EmbBtnOneHoldTimer
#define _EMBBTNHOLDTIMER embButtonHoldTimer
const unsigned int embButtonHoldTimer = EmbBtnOneHoldTimer;
#endif

#ifdef EmbBtnOneReleaseTimer
#define _EMBBTNRELEASETIMER embButtonReleaseTimer
const unsigned int embButtonReleaseTimer = EmbBtnOneReleaseTimer;
#endif

typedef enum
{
  EMB_BTN_STATE_RELEASED = -1,
  EMB_BTN_STATE_AWAIT = 0,
  EMB_BTN_STATE_PRESSED,
  EMB_BTN_STATE_HELD
} embButtonState;

typedef enum
{
  EMB_BTN_PRESS_NONE = -1,
  EMB_BTN_PRESS_CLICK = 0,
  EMB_BTN_PRESS_HOLDING
} embButtonPressType;

typedef struct
{
  char isClicked;
  char isReleased;
  char isHold;
  char endClicks;

#ifndef EmbBtnDisableDebounce
  char _lastState;
#endif

  unsigned short clicks;

  unsigned int _lastChange;

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
#ifndef EmbBtnOneDebTimer
#define _EMBBTNDEBTIMER btn->debounceTime
  unsigned int debounceTime;
#endif
#endif
  embButtonState state;
  embButtonPressType lastPressType;

  char (*buttonCheck) ();
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

void embButtonTick(embButton_t *btn)
{
  if (!btn)
    return;

  if (!btn->buttonCheck || !_EMBBTNMILLISFUNC)
  {
    return;
  }
  btn->isClicked = 0;
  btn->isHold = 0;
  btn->isReleased = 0;
  btn->lastPressType = EMB_BTN_PRESS_NONE;

  if (btn->endClicks)
  {
    btn->clicks = 0;
    btn->endClicks = 0;
  }

  char _pressed = 0;
  unsigned long t = _EMBBTNMILLISFUNC();
  char reading = btn->buttonCheck();
#ifndef EmbBtnDisableDebounce
  if (reading != btn->_lastState)
  {
    btn->_lastChange = t;
  }
  if (t - btn->_lastChange > _EMBBTNDEBTIMER)
  {
#endif
    _pressed = reading;
    if (_pressed)
    {
      switch (btn->state)
      {
        case EMB_BTN_STATE_AWAIT:
          btn->state = EMB_BTN_STATE_PRESSED;
          btn->timer = t;
          btn->clicks = 1;
          btn->isClicked = 1;
          break;

        case EMB_BTN_STATE_PRESSED:
          if (t - btn->timer >= _EMBBTNHOLDTIMER)
          {
            btn->state = EMB_BTN_STATE_HELD;
            btn->isHold = 1;
          }
          break;

        case EMB_BTN_STATE_RELEASED:
          btn->state = EMB_BTN_STATE_PRESSED;
          btn->timer = t;
          btn->clicks++;
          btn->isClicked = 1;
          break;
      }
    }
    else
    {
      switch (btn->state)
      {
        case EMB_BTN_STATE_PRESSED:
          btn->isReleased = 1;
          btn->lastPressType = EMB_BTN_PRESS_CLICK;
          if (btn->clicks >= EmbBtnMaxClicks)
          {
            btn->state = EMB_BTN_STATE_AWAIT;
            btn->endClicks = 1;
          }
          else
          {
            btn->state = EMB_BTN_STATE_RELEASED;
            btn->timer = t;
          }
          break;

        case EMB_BTN_STATE_HELD:
          btn->isReleased = 1;
          btn->lastPressType = EMB_BTN_PRESS_HOLDING;
#ifndef EmbBtnEndClicksAfterHolding
          if (btn->clicks >= EmbBtnMaxClicks)
          {
            btn->state = EMB_BTN_STATE_AWAIT;
            btn->endClicks = 1;
          }
          else
#endif
          {
            btn->state = EMB_BTN_STATE_RELEASED;
            btn->timer = t;
          }
          break;

        case EMB_BTN_STATE_RELEASED:
          if (t - btn->timer >= _EMBBTNRELEASETIMER)
          {
            btn->state = EMB_BTN_STATE_AWAIT;
            btn->timer = t;
            btn->endClicks = 1;
          }
          break;
      }
    }
#ifndef EmbBtnDisableDebounce
  }
  btn->_lastState = reading;
#endif
#ifdef EmbBtnUseActionCallbacks
  if (btn->isClicked)
    btn->clickedCallback();
  if (btn->isHold)
    btn->heldCallback();
  if (btn->isReleased)
    btn->releasedCallback();
  if (btn->endClicks)
    btn->endClicksCallback();
#endif
};

#endif
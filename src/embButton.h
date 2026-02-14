#ifndef __EMBBUTTON_H__
#define __EMBBUTTON_H__

//EmbButton by st3p40
//06.02.26

#ifndef EmbBtnMaxClicks
#define EmbBtnMaxClicks 254
#endif

#ifndef EmbBtnDefaultDebTimer
#define EmbBtnDefaultDebTimer 50
#endif
#ifndef EmbBtnDefaultHoldTimer
#define EmbBtnDefaultHoldTimer 500
#endif
#ifndef EmbBtnDefaultReleaseTimer
#define EmbBtnDefaultReleaseTimer 500
#endif

#ifdef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC embButtonMillisFunc
unsigned long (*embButtonMillisFunc) ();
#endif

#ifdef EmbBtnOneDebTimer
#define _EMBBTNDEBTIMER embButtonDebTimer
const unsigned int embButtonDebTimer=EmbBtnDefaultDebTimer;
#endif

#ifdef EmbBtnOneHoldTimer
#define _EMBBTNHOLDTIMER embButtonHoldTimer
const unsigned int embButtonHoldTimer=EmbBtnDefaultHoldTimer;
#endif

#ifdef EmbBtnOneReleaseTimer
#define _EMBBTNRELEASETIMER embButtonReleaseTimer
const unsigned int embButtonReleaseTimer=EmbBtnDefaultReleaseTimer;
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
    char isClicked = 0;
    char isReleased = 0;
    char isHold = 0;
    char endClicks = 0;

#ifndef EmbBtnDisableDebounce
    char _lastState = 0;
#endif

    unsigned short clicks = 0;

    unsigned int _lastChange = 0;

    unsigned int timer = 0;
#ifndef EmbBtnOneHoldTimer
#define _EMBBTNHOLDTIMER btn->holdTime
    unsigned int holdTime = EmbBtnDefaultHoldTimer;
#endif
#ifndef EmbBtnOneReleaseTimer
#define _EMBBTNRELEASETIMER btn->releaseTime
    unsigned int releaseTime = EmbBtnDefaultReleaseTimer;
#endif
#ifndef EmbBtnOneDebTimer || EmbBtnDisableDebounce
#define _EMBBTNDEBTIMER btn->debounceTime
    unsigned int debounceTime = EmbBtnDefaultDebTimer;
#endif
    embButtonState state = EMB_BTN_STATE_AWAIT;
    embButtonPressType lastPressType = EMB_BTN_PRESS_NONE;

    char (*buttonCheck) ();
#ifndef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC btn->millisFunc
    unsigned long (*millisFunc) ();
#endif
} embButton_t;

void embButtonTick(embButton_t *btn)
{
      if (!btn)
        return;

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
};

#endif
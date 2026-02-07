#ifndef __EMBBUTTON_H__
#define __EMBBUTTON_H__

//EmbButton by st3p40
//06.02.26

#ifdef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC embButtonMillisFunc
unsigned long (*embButtonMillisFunc) ();
#endif

typedef enum embButtonState {
  AWAIT = 0,
  PRESSED,
  HELD,
  RELEASED = -1
};

typedef enum embButtonPressType {
  CLICK = 0,
  HOLDING,
  NONE = -1
};

typedef struct embButton_t
{
    char isClicked;
    char isReleased;
    char isHold;

    char _lastState;
    
    char clicks;
    char endClicks;

    unsigned int _lastChange;

    unsigned int timer;
    unsigned int holdTime;
    unsigned int releaseTime;
    unsigned int debounceTime;

    embButtonState state;
    embButtonPressType lastPressType;
    
    char (*buttonCheck) ();
#ifndef EmbBtnOneMillisFunc
#define _EMBBTNMILLISFUNC btn->millisFunc
    unsigned long (*millisFunc) ();
#endif
} embButton_t;

void embButtonTick(embButton_t *btn)
{
      btn->isClicked = 0;
      btn->isHold = 0;
      btn->isReleased = 0;
      btn->endClicks = 0;
      btn->lastPressType = NONE;

      char _pressed = 0;
      unsigned long t = _EMBBTNMILLISFUNC();
      char reading = btn->buttonCheck();
      if (reading != btn->_lastState)
      {
          btn->_lastChange = t;
      }
      if (t - btn->_lastChange > btn->debounceTime)
      {
        _pressed = !reading;
        if (_pressed)
        {
          switch (btn->state)
          {
            case AWAIT:
              btn->state = PRESSED;
              btn->timer = t;
              btn->clicks = 1;
              btn->isClicked = 1;
              break;

            case PRESSED:
              if (t - btn->timer >= btn->holdTime)
              {
                btn->state = HELD;
                btn->isHold = 1;
              }
            break;

          case RELEASED:
            btn->state = PRESSED;
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
          case PRESSED:  
            btn->isReleased = 1;
            btn->lastPressType = CLICK;
            if (btn->clicks >= 254)
            {
              btn->state = AWAIT;
              btn->endClicks = btn->clicks;
              btn->clicks = 0;
            }
            else
            { 
              btn->state = RELEASED; 
              btn->timer = t;
            }
          break;

          case HELD:
            btn->isReleased = 1;
            btn->lastPressType = HOLDING;
            if (btn->clicks >= 254)
            {
              btn->state = AWAIT;
              btn->endClicks = btn->clicks;
              btn->clicks = 0;
            }
            else
            { 
              btn->state = RELEASED; 
              btn->timer = t;
            }
          break;

          case RELEASED:
            if (t - btn->timer >= btn->releaseTime)
            {
              btn->state = AWAIT;
              btn->timer = t;
              btn->endClicks = btn->clicks;
              btn->clicks = 0;
            }
          break;
        }
      }    
    }
  btn->_lastState = reading;
};


#endif

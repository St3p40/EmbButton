#ifndef __EMBBUTTON_H__
#define __EMBBUTTON_H__

//                                .::.
//                           =@@@@%**#@@@@+
//                       #@@@+            =@@@#.           @@
//                   -@%=       -@@@@@@=       =%@-        @@
//    +%@%+         *%#        -@@@@@@@@-        #%#       @@  =@@@=
//  @@=   =@@       #@%  #        @@@@         # %@%       @@@@=  =@@
// %@@@@@@@@@%      %@@                          @@%       @@@     +@%
// @@               @@@                          @@@       @@@.    :@@
// :@@    =@@  =@*  %@+       @@@@@@@@@@@@       +@@       @@@@-+@@@@-
//    =@@=     =@#                +@@*                     @@# .=--
//             *@%:               -@@=      =#%%*
//             %@@@@@ #@*  +@      %%      @@. @@% %@#=
//             @@* %@ @@*  +@.            .@@  *@@ @@*@@@
//             @@@@@# %@%  %@-  ++%@@%++  :@%  #@% @@ -@@
//             .+=    .@@-.@@.     @@.    .@@:-@@.    =@@
//                      .++-       ##       -++.

//Made by St3p40
//06.02.26


//Max clicks
#ifndef EMBBTN_MAX_CLICKS
#define EMBBTN_MAX_CLICKS 7
#endif

#ifndef EMBBTN_CLICKS_VAR_SIZE
#define EMBBTN_CLICKS_VAR_SIZE 3
#endif

#if EMBBTN_MAX_CLICKS > ((1 << EMBBTN_CLICKS_VAR_SIZE) - 1)
#error EmbButton: EMBBTN_MAX_CLICKS is set without EMBBTN_CLICKS_VAR_SIZE or EMBBTN_MAX_CLICKS exceeds EMBBTN_CLICKS_VAR_SIZE capacity
#endif

//Input info
#ifdef EMBBTN_MILLIS_FOR_EACH_BTN
#define _EMBBTN_MILLIS_NAME millis
#define _EMBBTN_MILLIS_PREF btn->
#else
#define _EMBBTN_MILLIS_NAME embButtonMillis
#define _EMBBTN_MILLIS_PREF
#endif

#ifndef EMBBTN_BTN_READ
#define EMBBTN_BTN_READ CLBK
#endif

#ifndef EMBBTN_BTN_MILLIS
#define EMBBTN_BTN_MILLIS CLBK
#endif

#if EMBBTN_BTN_READ == CLBK
#define _EMBBTN_BTN_DECLARATION unsigned char (*buttonCheck) (void)
#define _EMBBTN_BTN_READ btn->buttonCheck()
#elif EMBBTN_BTN_READ == PNTR
#define _EMBBTN_BTN_DECLARATION unsigned char* buttonCheck
#define _EMBBTN_BTN_READ *btn->buttonCheck
#elif EMBBTN_BTN_READ == VRBL
#define _EMBBTN_BTN_DECLARATION unsigned char buttonCheck
#define _EMBBTN_BTN_READ btn->buttonCheck
#else
#error EmbButton: EMBBTN_BTN_READ definition as macros isn't implemented
#endif

#if EMBBTN_BTN_MILLIS == CLBK
#define _EMBBTN_MILLIS_DECLARATION unsigned long (*_EMBBTN_MILLIS_NAME) (void)
#define _EMBBTN_MILLIS_READ _EMBBTN_MILLIS_PREF _EMBBTN_MILLIS_NAME ()
#elif EMBBTN_BTN_MILLIS == PNTR
#define _EMBBTN_MILLIS_DECLARATION unsigned long* _EMBBTN_MILLIS_NAME
#define _EMBBTN_MILLIS_READ * _EMBBTN_MILLIS_PREF _EMBBTN_MILLIS_NAME
#elif EMBBTN_BTN_MILLIS == VRBL
#define _EMBBTN_MILLIS_DECLARATION unsigned long buttonCheck
#define _EMBBTN_MILLIS_READ _EMBBTN_MILLIS_PREF _EMBBTN_MILLIS_NAME
#else
#ifndef EMBBTN_MILLIS_FOR_EACH_BTN
#define _EMBBTN_MILLIS_READ EmbBtnMillis
#else
#error EmbButton: EMBBTN_BTN_MILLIS can't be as macros with EMBBTN_MILLIS_FOR_EACH_BTN
#endif
#endif

#ifndef EMBBTN_MILLIS_FOR_EACH_BTN
_EMBBTN_MILLIS_DECLARATION;
#endif

//Timers
#ifdef EMBBTN_GLOBAL_HLD_TIMER
#define _EMBBTN_HLD_T EMBBTN_GLOBAL_HLD_TIMER
#endif

#ifdef EMBBTN_GLOBAL_RLS_TIMER
#define _EMBBTN_RLS_T EMBBTN_GLOBAL_RLS_TIMER
#endif

#if defined(EMBBTN_GLOBAL_DEB_TIMER) && !defined(EMBBTN_DISABLE_DEBOUNCE)
#define _EMBBTN_DEB_T EMBBTN_GLOBAL_DEB_TIMER
#endif

#if defined(EMBBTN_GLOBAL_STP_TIMER) && defined(EMBBTN_ENABLE_STEP)
#define _EMBBTN_STP_T EMBBTN_GLOBAL_STP_TIMER
#endif


//Typedefs
typedef enum
{
  EMBBTN_STATE_IDL = 0,
  EMBBTN_STATE_RLS,
  EMBBTN_STATE_PRS,
  EMBBTN_STATE_HLD
} embButtonState;

typedef struct
{
  //flags
  unsigned int clicked       :1;
  unsigned int released      :1;
  unsigned int held          :1;
  unsigned int endClicking   :1;
#ifndef EMBBTN_DISABLE_DEBOUNCE
  unsigned int _lastState    :1;
#endif
#ifdef EMBBTN_ENABLE_STEP
   unsigned int step         :1;
#endif
  //misc
  unsigned int lastPressType :1;
  //state(IDLE, RELEASED, PRESSED, HOLD)
  embButtonState state       :2;
  //
  unsigned int clicks        :EMBBTN_CLICKS_VAR_SIZE;
  //timestamps
#ifndef EMBBTN_DISABLE_DEBOUNCE
  unsigned long _lastChange;
#endif
  unsigned long timer;
#ifdef EMBBTN_ENABLE_STEP
  unsigned long _stepTimer;
#endif
} embButtonStats;

typedef struct
{

embButtonStats s;

#ifndef EMBBTN_GLOBAL_HLD_TIMER
#define _EMBBTN_HLD_T btn->holdTime
  unsigned int holdTime;
#endif

#ifndef EMBBTN_GLOBAL_RLS_TIMER
#define _EMBBTN_RLS_T btn->releaseTime
  unsigned int releaseTime;
#endif

#if !defined(EMBBTN_DISABLE_DEBOUNCE) && !defined(EMBBTN_GLOBAL_DEB_TIMER)
#define _EMBBTN_DEB_T btn->debounceTime
  unsigned int debounceTime;
#endif

#if defined(EMBBTN_ENABLE_STEP) && !defined(EMBBTN_GLOBAL_STP_TIMER)
#define _EMBBTN_STP_T btn->stepTime
  unsigned int stepTime;
#endif

_EMBBTN_BTN_DECLARATION;

#ifdef EMBBTN_MILLIS_FOR_EACH_BTN
_EMBBTN_MILLIS_DECLARATION;
#endif

#ifdef EMBBTN_USE_ACTION_CALLBACKS
  void (*clickedCallback) (void *);
  void (*heldCallback) (void *);
  void (*releasedCallback) (void *);
  void (*endClicksCallback) (void *);
#endif
} embButton_t;

void embButtonTick(embButton_t *btn);
#ifdef EMBBTN_USE_ACTION_CALLBACKS
void embButtonActionCallback(embButton_t *btn);
#endif

//Button reading defines
#ifdef EMBBTN_READING_DEFS
#define eb_isPressed(x) x.s.state >= EMBBTN_STATE_PRS
#define eb_isClicked(x) x.s.clicked
#define eb_isReleased(x) x.s.released
#define eb_isHeld(x) x.s.held
#define eb_onHold(x) x.s.state = EMBBTN_STATE_HLD
#define eb_hasClicks(x) (!x.s.endClicking) ? x.s.clicks : 0
#define eb_hadClicks(x) (x.s.endClicking) ? x.s.clicks : 0
#define eb_wasClick(x) x.s.released && !x.s.lastPressType
#define eb_wasHold(x) x.s.released && x.s.lastPressType
#define eb_pressTime(x, mls) (x.s.state >= EMBBTN_STATE_PRS)? mls - x.s.timer : 0
#define eb_releaseTime(x, mls) (x.s.state < EMBBTN_STATE_PRS)? mls - x.s.timer : 0
#define eb_fullPressTime(x, mls) (x.s.released)? mls - x.s.timer : 0
#define eb_fullIdleTime(x, mls) (x.s.clicked)? mls - x.s.timer : 0
#ifdef EMBBTN_ENABLE_STEP
#define eb_isStep(x) x.s.step
#define eb_stepTime(x, mls) (x.s.state == EMBBTN_STATE_HLD)? mls - _stepTimer : 0
#endif
#endif

#ifdef EMBBTN_HANDLE_MULTIPLE_BTNS
#define embButtonsTick(btns, count) \
    for(size_t i = 0; i < count; i++) embButtonTick(&btns[i])
#ifdef EMBBTN_USE_ACTION_CALLBACKS
#define embButtonsActionCallback(btns, count) \
    for(size_t i = 0; i < count; i++) embButtonActionCallback(&btns[i])
#endif
#endif

#endif

#ifdef EMBBTN_FUNCTIONS_IMPLEMENTATION
//Functions
void embButtonTick(embButton_t *btn)
{
  if (!btn)
    return;

  unsigned long t = _EMBBTN_MILLIS_READ;
  char reading = _EMBBTN_BTN_READ;

  btn->s.held = 0;
#ifdef EMBBTN_ENABLE_STEP
  btn->s.step = 0;
#endif
  if (btn->s.clicked)
  {
    btn->s.timer = t;
    btn->s.clicked = 0;
  }
  if (btn->s.released)
  {
    btn->s.timer = t;
    btn->s.released = 0;
  }
  if (btn->s.endClicking)
  {
    btn->s.clicks = 0;
    btn->s.endClicking = 0;
  }

#ifndef EMBBTN_DISABLE_DEBOUNCE
  if (reading != btn->s._lastState)
  {
    btn->s._lastChange = t;
  }
  if (t - btn->s._lastChange > _EMBBTN_DEB_T)
  {
#endif
    if (reading)
    {
      switch (btn->s.state)
      {
        case EMBBTN_STATE_IDL: case EMBBTN_STATE_RLS:
          btn->s.state = EMBBTN_STATE_PRS;
          btn->s.clicks++;
          btn->s.clicked = 1;
        break;

        case EMBBTN_STATE_PRS:
          if (t - btn->s.timer >= _EMBBTN_HLD_T)
          {
            btn->s.state = EMBBTN_STATE_HLD;
            btn->s.held = 1;
#ifdef EMBBTN_ENABLE_STEP
            btn->s._stepTimer = t;
#endif
          }
          break;
#ifdef EMBBTN_ENABLE_STEP
        case EMBBTN_STATE_HLD:
          if (t - btn->s._stepTimer >= _EMBBTN_STP_T)
          {
            btn->s.step = 1;
            btn->s._stepTimer = t;
          }
          break;
#endif
      }
    }
    else
    {
      switch (btn->s.state)
      {
        case EMBBTN_STATE_PRS: case EMBBTN_STATE_HLD:
          btn->s.released = 1;
          btn->s.lastPressType = btn->s.state == EMBBTN_STATE_HLD;
#ifdef EMBBTN_ENDCLICKING_AFTER_HOLD
          if (btn->s.clicks >= EMBBTN_MAX_CLICKS || btn->s.state == EMBBTN_STATE_HLD)
          {
#else
          if (btn->s.clicks >= EMBBTN_MAX_CLICKS)
          {
#endif
            btn->s.state = EMBBTN_STATE_IDL;
            btn->s.endClicking = 1;
          }
          else
          {
            btn->s.state = EMBBTN_STATE_RLS;
          }
          break;

        case EMBBTN_STATE_RLS:
          if (t - btn->s.timer >= _EMBBTN_RLS_T)
          {
            btn->s.state = EMBBTN_STATE_IDL;
            btn->s.endClicking = 1;
          }
         break;
      }
    }
#ifndef EMBBTN_DISABLE_DEBOUNCE
  }
  btn->s._lastState = reading;
#endif
}

#ifdef EMBBTN_USE_ACTION_CALLBACKS
void embButtonActionCallback(embButton_t *btn)
{
  if (!btn)
    return;
  if (btn->s.clicked && btn->clickedCallback)
    btn->clickedCallback(btn);
  if (btn->s.held && btn->heldCallback)
    btn->heldCallback(btn);
  if (btn->s.released && btn->releasedCallback)
    btn->releasedCallback(btn);
  if (btn->s.endClicking && btn->endClicksCallback)
    btn->endClicksCallback(btn);
}
#endif
#endif // EMBBTN_USE_ACTION_CALLBACKS

#undef _EMBBTN_MILLIS_NAME
#undef _EMBBTN_MILLIS_PREF
#undef _EMBBTN_BTN_DECLARATION
#undef _EMBBTN_BTN_READ
#undef _EMBBTN_MILLIS_DECLARATION
#undef _EMBBTN_MILLIS_READ
#undef _EMBBTN_HLD_T
#undef _EMBBTN_RLS_T
#undef _EMBBTN_DEB_T
#undef _EMBBTN_STP_T
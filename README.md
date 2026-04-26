# EmbButton
![License](https://img.shields.io/badge/License-MIT-blue)
![Tests](https://img.shields.io/github/languages/top/St3p40/EmbButton)
![Tests](https://img.shields.io/github/last-commit/St3p40/EmbButton)

EmbButton is a lightweight, header-only, platform-independent button handling library for embedded systems. It provides sophisticated button event detection including clicks, holds, multi-click sequences, and debouncing through a single header file with no external dependencies.

## Configuration
### Default way
```c
// !!! NEED TO BE DEFINED ON .c/c++ file once before including the library
#define EMBBTN_FUNCTIONS_IMPLEMENTATION
//
embButton_t btn;                       // Creating a variable for button

btn.debounceTime=100;                  // Setting button's min stable press time
btn.releaseTime=500;                   // Setting button's idle to await time
btn.holdTime=1000;                     // Setting button's hold duration treshold

embButtonMillis = millis;              // Assign system time source for all buttons

btn.buttonCheck=btn_read;              // Assign read pin function
```
> [!IMPORTANT]
> All defines are need to be defined before including the library
### Changes with defines
```c
#define EMBBTN_MILLIS_FOR_EACH_BTN
btn.millis=millis;                     // Assign system time source for each button

#define EMBBTN_BTN_MILLIS <CLBK/PNTR/VRBL>
#define EMBBTN_BTN_READ   <CLBK/PNTR/VRBL>
//
//CLBK - assign callback
//PNTR - assign pointer to the variable
//VRBL - create variable that will be set manualy
//#define EMBBTN_BTN_... ...  - custom MACROS
//

#define EMBBTN_GLOBAL_DEB_TIMER <value> // debounceTime
#define EMBBTN_GLOBAL_HLD_TIMER <value> // holdTime
#define EMBBTN_GLOBAL_RLS_TIMER <value> // releaseTime

#define EMBBTN_ENABLE_STEP              // Enables step
#define EMBBTN_GLOBAL_STP_TIMER <value> // stepTime

#define EMBBTN_DISABLE_DEBOUNCE         // Deletes debounce variables and it's logic in code
```
## Variables
### States
```c
embButtonState s.state;
state=EMBBTN_STATE_IDL; // 0           // waiting, no activity detected
state=EMBBTN_STATE_RLS; // 1           // idle, waiting for a new possible click or releaseTime
state=EMBBTN_STATE_PRS; // 2           // button is pressed less than holdTime
state=EMBBTN_STATE_HLD; // 3           // button is pressed more than holdTime

```
### Event flags
```c
unsigned int s.clicked;                // button clicks event
unsigned int s.released;               // button release event
unsigned int s.held;                   // button's holdTime event
unsigned int s.endClicking;            // multi-click finalizer
#define EMBBTN_ENABLE_STEP
unsigned int s.step;                   // if stepTimer is triggered
```
### Other
```c
unsigned int s.lastPressType;          // last interaction type(press or hold)
unsigned long s.timer;                 // reference timestamp for measuring press and idle time
unsigned int s.clicks;                 // clicks amount before endclicks

unsigned int s._lastState;             // previous raw state for debounce
unsigned int s._lastChange;            // timestamp of last state change

#define EMBBTN_ENABLE_STEP
unsigned int stepTime;                 // timestamp for holding steps
```
### Action callbacks
```c
#define EMBBTN_USE_ACTION_CALLBACKS
void (*clickedCallback) (void *);      // Triggers when s.clicked
void (*heldCallback) (void *);         // Triggers when s.held
void (*releasedCallback) (void *);     // Triggers when s.released
void (*endClicksCallback) (void *);    // Triggers when s.endCliking
```
## Functions
```c
embButtonTick(&btn);                   // Checks button's state and updates its' variables(only 1 button)

#define EMBBTN_USE_ACTION_CALLBACKS
embButtonaActionCallback(&btn);        // Event dispatcher (executes action callbacks)
```
## Misc defines
```c
#define EMBBTN_MAX_CLICKS <value>      // Sets max clicks before automaticaly endClicks event
#define EMBBTN_CLICKS_VAR_SIZE <value> // Sets clicks variable size(in bits)
#define EMBBTN_ENDCLICKING_AFTER_HOLD  // (EMFButton'ish logic) endClicking event after holding
#define EMBBTN_HANDLE_MULTIPLE_BTNS    // adds macroses to handle more than one button at once
#define EMBBTN_READING_DEFS            // adds macroses to read button easely
```
### Different macroses
```c
#define EMBBTN_HANDLE_MULTIPLE_BTNS    // embButtons...
embButtonsTick(btns, count)
#define EMBBTN_USE_ACTION_CALLBACKS
embButtonsActionCallback(btns, count)

#define EMBBTN_READING_DEFS             // eb_...
eb_isPressed(x) x.s.state >= EMBBTN_STATE_PRS
eb_isClicked(x) x.s.clicked
eb_isReleased(x) x.s.released
eb_isHeld(x) x.s.held
eb_onHold(x) x.s.state = EMBBTN_STATE_HLD
eb_hasClicks(x) (!x.s.endClicking) ? x.s.clicks : 0
eb_hadClicks(x) (x.s.endClicking) ? x.s.clicks : 0
eb_wasClick(x) x.s.released && !x.s.lastPressType
eb_wasHold(x) x.s.released && x.s.lastPressType
eb_pressTime(x, mls) (x.s.state >= EMBBTN_STATE_PRS)? mls - x.s.timer : 0
eb_releaseTime(x, mls) (x.s.state < EMBBTN_STATE_PRS)? mls - x.s.timer : 0
eb_fullPressTime(x, mls) (x.s.released)? mls - x.s.timer : 0
eb_fullIdleTime(x, mls) (x.s.clicked)? mls - x.s.timer : 0
#define EMBBTN_ENABLE_STEP
eb_isStep(x) x.s.step
eb_stepTime(x, mls) (x.s.state == EMBBTN_STATE_HLD)? mls - _stepTimer : 0
```

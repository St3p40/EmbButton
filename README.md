# EmbButton
![License](https://img.shields.io/badge/License-MIT-blue)
![Tests](https://img.shields.io/github/languages/top/St3p40/EmbButton)
![Tests](https://img.shields.io/github/last-commit/St3p40/EmbButton)

EmbButton is a lightweight, header-only, platform-independent button handling library for embedded systems. It provides sophisticated button event detection including clicks, holds, multi-click sequences, and debouncing through a single header file with no external dependencies.

## Configuration
### Default way
```c
embButton_t btn;                      // Creating a variable for button

btn.debounceTime=100;                 // Setting button's min stable press time
btn.releaseTime=500;                  // Setting button's idle to await time
btn.holdTime=1000;                    // Setting button's hold duration treshold

btn.millisFunc=millis;                // Assign system time source
btn.buttonCheck=btn_read;             // Assign read pin function
```
### Changes with defines
```c
#define EmbBtnOneMillisFunc           // !Before including the library!
embButtonMillisFunc=millis;           // Assign system time source for all buttons

//!Before including the library! These 3 defines moves variables outside the button's struct and add them const attribute
#define EmbBtnOneDebTimer <value>     // debounceTime into embButtonDebTimer
#define EmbBtnOneHoldTimer <value>    // holdTime into embButtonHoldTimer
#define EmbBtnOneReleaseTimer <value> // releaseTime into embButtonReleaseTimer

#define EmbBtnDisableDebounce         // !Before including the library! Deletes debounce variables and it's logic in code
```
## Variables
### States
```c
embButtonState state;                 // !Before including the library!
state=EMB_BTN_STATE_AWAIT;            // waiting, no activity detected
state=EMB_BTN_STATE_PRESSED;          // button is pressed less than holdTime
state=EMB_BTN_STATE_HELD;             // button is pressed more than holdTime
state=EMB_BTN_STATE_RELEASED;         // idle, waiting for a new possible click or releaseTime
```
### Event flags
```c
char isClicked;                       // button clicks event
char isReleased;                      // button release event
char isHold;                          // button's holdTime event
char endClicks;                       // multi-click finalizer
```
### Other
```c
char lastPressType;                   // last interaction type(press or hold)
unsigned int timer;                   // reference timestamp for measuring press and idle time
unsigned short clicks;                // clicks amount before endclicks
char _lastState;                      // previous raw state for debounce
unsigned int _lastChange;             // timestamp of last state change
```
### Action callbacks
```c
#define EmbBtnUseActionCallbacks      // !Before including the library!
void (*clickedCallback) ();           // Triggers when isClicked
void (*heldCallback) ();              // Triggers when isHold
void (*releasedCallback) ();          // Triggers when isReleased
void (*endClicksCallback) ();         // Triggers when endCliks
```
## Functions
```c
embButtonTick(&btn);                  // Checks button's state and updates its' variables(only 1 button)

#define EmbBtnUseActionCallbacks      // !Before including the library!
embButtonaActionCallback(&btn);       // Event dispatcher (executes action callbacks)
```
## Misc defines
```c
//!Before including the library!
#define EmbBtnMaxClicks <value>       // Sets max clicks before automaticaly endClicks event
#define EmbBtnMaxClicksSize <value>   // Sets clicks variable size(in bits)
#define EmbBtnEndClicksAfterHolding   // (EMFButton'ish logic) endClicks event after holding
#define EmbButtonHandleMultipleButtons// multiple buttons handle with one function ( func(arrptr, amount) )
```

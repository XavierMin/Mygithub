// pinMap.h

#ifndef _PINMAP_h
#define _PINMAP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3  //IN
#define X_MAX_PIN           2  //IN

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14  //IN
#define Y_MAX_PIN          15  //IN

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18  //IN
#define Z_MAX_PIN          19  //IN

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24



#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define R_STEP_PIN         47
#define R_DIR_PIN          32
#define R_ENABLE_PIN       35
#define R_POWER_PIN        10



#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING


#endif

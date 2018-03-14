
#ifndef _DEFINES_H
#define _DEFINES_H

/** Firmware version, hardware version, and maximal values */
#define FWV    90    // Firmware version: 100 means 1.0.0

/** GPIO pins */
#define BRIDGE1_PIN 5     // D1 // Switch L293D In1
#define BRIDGE2_PIN 4     // D2 // Switch L293D In2
#define BRIDGE3_PIN 2     // D4 // Switch L293D In3
#define BRIDGE4_PIN 0     // D3 // Switch L293D In4

#define MAX_NUMBER_ZONES 2

#define LED_FAST_BLINK 100
#define LED_SLOW_BLINK 500
#define SERVER_PORT 82

#define TIME_SYNC_TIMEOUT  3600

/** Serial debug functions */
#define SERIAL_DEBUG
#if defined(SERIAL_DEBUG)

#define DEBUG_BEGIN(x)   { Serial.begin(x); }
#define DEBUG_PRINT(x)   Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)

#else

#define DEBUG_BEGIN(x)   {}
#define DEBUG_PRINT(x)   {}
#define DEBUG_PRINTLN(x) {}

#endif

typedef unsigned char byte;
typedef unsigned long ulong;

#endif  // _DEFINES_H

/*
 * global.h
 *
 *  Created on: Nov 22, 2013
 *      Author: bpatters
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdarg.h>
#include <stdio.h>
#define LED1_PIN 2
#define LED2_PIN 3

//#define DEBUG 1
//#define DEBUG_TRACE
#define TEST



#ifdef DEBUG_LOW
#define error(x) debug(x)
#define error(x,...) debug(x,__VA_ARGS__)
#define info(x) debug(x)
#define info(x,...)  debug(x,__VA_ARGS__)
#elif DEBUG_WARN
#define error(x) debug(x)
#define error(x,...) debug(x,__VA_ARGS__)
#define info(x) debug(x)
#define info(x,...)  debug(x,__VA_ARGS__)
#define warn(x)  debug(x)
#define warn(x,...)  debug(x,__VA_ARGS__)
#elif DEBUG_TRACE
#define error(x) debug(x)
#define error(x,...) debug(x,__VA_ARGS__)
#define info(x) debug(x)
#define info(x,...)  debug(x,__VA_ARGS__)
#define warn(x)  debug(x)
#define warn(x,...)  debug(x,__VA_ARGS__)
#define trace(x) debug(x);
#define trace(x,...) debug(x,__VA_ARGS__)
#else
#define error(x,...)
#define info(x,...)
#define warn(x,...)
#define trace(x,...)
#endif

#ifdef DEBUG
static inline void debug(char *fmt, ... ){
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    // NEED Print statement here
    printf (tmp);
}
#else
#define debuglog(...)
#endif

#define delay(x) nrf_delay_ms(x)

#endif /* GLOBAL_H_ */

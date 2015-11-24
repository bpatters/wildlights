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
#define LED1_PIN  2
#define POWER_PIN 3

#define DEVICE_NAME                          "WildLights"                       	   /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                    "BP"                     /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_INTERVAL                     40                                        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS           180                                       /**< The advertising timeout in units of seconds. */

#define APP_TIMER_PRESCALER                  0                                         /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS                 4                                         /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE              5                                         /**< Size of timer operation queues. */
// number of ticks in one millisecond
#define TICKS_PER_MILLIS          APP_TIMER_TICKS(1, APP_TIMER_PRESCALER)/**< Battery level measurement interval (ticks). */
#define delay(x) nrf_delay_ms(x)

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


#endif /* GLOBAL_H_ */

/**
 * @file net_prisoner_common.c
 * @author Thomas Violent & Wolodia Zdetovetzky
 * @brief 
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "net_prisoner_common.h"

// ----------------------------------------------
//                     Common
// ----------------------------------------------
#pragma region Common

/**
 * @brief Semaphore used for locking the _net_common_dbg function
 * This should prevent multiple thread from writing to STDOUT at the same time
 */
sem_t _lock_log_dbg;

/**
 * @brief show debug message for the net lib
 * in this lib, _net_common_dbg should be used insteadof printf directly.
 * this allow the #define NETDEBUG to easily turn on/off all message
 * from this library
 * 
 * @param format work exactly like printf
 * @param ... 
 */
void _net_common_dbg(const char *format, ...)
{
    if (NETDEBUG)
    {
        // grab the semaphore
        sem_wait(&_lock_log_dbg);

        printf("NET: ");

        // see: https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/printf.c;h=4c8f3a2a0c38ab27a2eed4d2ff3b804980aa8f9f;hb=3321010338384ecdc6633a8b032bb0ed6aa9b19a
        va_list arg;

        va_start(arg, format);

        // https://www.cplusplus.com/reference/cstdio/vfprintf/
        vfprintf(stdout, format, arg);

        va_end(arg);

        // release the semaphore
        sem_post(&_lock_log_dbg);
    }
}

/**
 * @brief initialize common variable for client and server, should be called
 * in client and server init function.
 */
void _net_common_init()
{
    // initialize the semaphore before use
    sem_init(&_lock_log_dbg, PTHREAD_PROCESS_SHARED, 1);
    if (THREAD_SAFETY)
    {
        _net_common_dbg("Thread safety enabled");
    }
}
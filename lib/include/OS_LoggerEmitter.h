/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Implementation of concrete log emitter.
 */
#pragma once

/**
 * @defgroup    OS_LoggerEmitter Log emitter implementation
 *
 * @brief       This layer provides a implementation of concrete log emitter.
 * @details     On the client side, a log emitter is the object that transceives
 *              log messages to a server.
 *
 *              A separate log emitter object must be created for each client.
 *              It will be automatically created by calling a Debug_LOG
 *              function.
 *              \n \n
 *              Log emitter starts to transceives log messages, when it got the
 *              signal from the server.
 *
 *              The log emitter copy the log message and the client log level in
 *              the given data buffer.
 *
 *              Afterwards it sends a signal to server.
 *              \n \n
 *              The log emitter object is the central object that offers
 *              interfaces for:
 *              \code
 *                  log filter
 *                  flow control functions
 *              \endcode
 *
 *              The latter is the class \link log_emitter_callback \endlink .
 *
 * @see         log_emitter_callback
 *
 * @ingroup     OS_LoggerClient
*/
#include "OS_LoggerFilter.h"
#include "OS_LoggerEmitterCallback.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>


/**
 * @details OS_LoggerEmitter_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef struct OS_LoggerEmitter_Handle OS_LoggerEmitter_Handle_t;


/**
 * @details OS_LoggerEmitter_dtor_t defines the interface for function pointer
 *          to destructor.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef void
(*OS_LoggerEmitter_dtor_t)(void);


/**
 * @details OS_LoggerEmitter_getBuffer_t defines the interface for function
 *          pointer to get client data buffer.
 *
 * @return  pointer to data buffer
 *
 * @ingroup OS_LoggerEmitter
*/
typedef void*
(*OS_LoggerEmitter_getBuffer_t)(void);


/**
 * @details OS_LoggerEmitter_wait_t defines the interface for function pointer
 *          to wait for signal from server.
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerEmitter
*/
typedef bool
(*OS_LoggerEmitter_wait_t)(void);


/**
 * @details OS_LoggerEmitter_emit_t defines the interface for function pointer
 *          to emit a signal to server.
 *
 *          The signal meant the logging process is complete and the log server
 *          can process the log.
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerEmitter
*/
typedef bool
(*OS_LoggerEmitter_emit_t)(void);


/**
 * @details OS_LoggerEmitter_log_t defines the interface for function pointer to
 *          process the client log action.
 *
 *          This function is called by the debug log interface function
 *          Debug_LOG _XXX.
 *
 *          The function has variable parameters so that a log message can be
 *          called up as usual with the syntax of the printf function.
 *
 * @param   log_level:  client log level id
 * @param   format:     log message
 * @param   ...:        optional parameters for log message
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerEmitter
*/
typedef bool
(*OS_LoggerEmitter_log_t)(uint8_t log_level, const char* format, ...);


/**
 * @details OS_LoggerEmitter_vtable_t contain the member functions to his class.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef struct
{
    OS_LoggerEmitter_dtor_t      dtor;
    OS_LoggerEmitter_getBuffer_t get_buffer;
    OS_LoggerEmitter_wait_t      wait;
    OS_LoggerEmitter_emit_t      emit;
    OS_LoggerEmitter_log_t       log;
} OS_LoggerEmitter_vtable_t;


/**
 * @details OS_LoggerEmitter_Handle contain the vtable to his class.
 *
 * @ingroup OS_LoggerEmitter
*/
struct OS_LoggerEmitter_Handle
{
    void*                                buf;
    OS_LoggerFilter_Handle_t*                   log_filter;
    OS_LoggerEmitterCallback_Handle_t* callback_vtable;
    const OS_LoggerEmitter_vtable_t*     vtable;
};


/**
 * @details %OS_LoggerEmitter_getInstance is a getter to instantiate this object.
 *
 *          This is an singleton implementation i.e. it will internally create a
 *          log emitter object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, otherwise this functions will return a NULL
 *          pointer.
 *
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @param   buffer:             data buffer
 * @param   log_filter:         layer for log filter
 * @param   callback_vtable:    layer for callback vtable
 *
 * @return  pointer to OS_LoggerEmitter_Handle_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitter
*/
OS_LoggerEmitter_Handle_t*
OS_LoggerEmitter_getInstance(
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter,
    OS_LoggerEmitterCallback_Handle_t* callback_vtable);


/**
 * @details %OS_LoggerEmitter_dtor is the destructor.
 *
 * @ingroup OS_LoggerEmitter
*/
void
OS_LoggerEmitter_dtor(void);


/**
 * @details %OS_LoggerEmitter_log provides the client log action.
 *
 *          This function is called by the debug log interface function
 *          Debug_LOG _XXX.
 *
 *          The function has variable parameters so that a log message can be
 *          called up as usual with the syntax of the printf function.
 *
 * @param   log_level:  client log level id
 * @param   format:     log message
 * @param   ...:        optional parameters for log message
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitter
*/
bool
OS_LoggerEmitter_log(uint8_t log_level, const char* format, ...);

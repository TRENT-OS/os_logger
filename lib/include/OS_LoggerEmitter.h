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
 * @details Log_emitter_t defines the class datatype.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef struct Log_emitter_t Log_emitter_t;


/**
 * @details Log_emitter_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef void
(*Log_emitter_dtorT)(void);


/**
 * @details Log_emitter_get_bufferT defines the interface for function pointer
 *          to get client data buffer.
 *
 * @return  pointer to data buffer
 *
 * @ingroup OS_LoggerEmitter
*/
typedef void*
(*Log_emitter_get_bufferT)(void);


/**
 * @details Log_emitter_waitT defines the interface for function pointer to wait
 *          for signal from server.
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerEmitter
*/
typedef bool
(*Log_emitter_waitT)(void);


/**
 * @details Log_emitter_emitT defines the interface for function pointer to emit
 *          a signal to server.
 *
 *          The signal meant the logging process is complete and the log server
 *          can process the log.
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerEmitter
*/
typedef bool
(*Log_emitter_emitT)(void);


/**
 * @details Log_emitter_emit_logT defines the interface for function pointer to
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
(*Log_emitter_emit_logT)(uint8_t log_level, const char* format, ...);


/**
 * @details Log_emitter_Vtable contain the member functions to his class.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef struct
{
    Log_emitter_dtorT       dtor;
    Log_emitter_get_bufferT get_buffer;
    Log_emitter_waitT       wait;
    Log_emitter_emitT       emit;
    Log_emitter_emit_logT   emit_log;
} Log_emitter_Vtable;


/**
 * @details Log_emitter_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerEmitter
*/
struct Log_emitter_t
{
    void*                     buf;             /**< data buffer */
    Log_filter_t*             log_filter;      /**< layer for log filter */
    Log_emitter_callback_t*   callback_vtable; /**< layer for callback vtable */
    const Log_emitter_Vtable* vtable;          /**< vtable */
};


/**
 * @details %get_instance_Log_emitter is a getter to instantiate this object.
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
 * @return  pointer to Log_emitter_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitter
*/
Log_emitter_t*
get_instance_Log_emitter(void* buffer,
                         Log_filter_t* log_filter,
                         Log_emitter_callback_t* callback_vtable);


/**
 * @details %Log_emitter_dtor is the destructor.
 *
 * @ingroup OS_LoggerEmitter
*/
void
Log_emitter_dtor(void);


/**
 * @details %Log_emitter_emit_log provides the client log action.
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
Log_emitter_emit_log(uint8_t log_level, const char* format, ...);

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log emitter flow control interface.
 */
#pragma once

/**
 * @defgroup    OS_LoggerEmitterCallback Log emitter flow control interface
 *
 * @brief       This layer provides functions for log emitter flow control.
 * @details     The functions of this level serve to control the flow of log
 *              emitters.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     OS_LoggerClient
*/
#include "OS_LoggerFilter.h"
#include <stdarg.h>
#include <stdbool.h>

/**
 * @details Log_emitter_callback_emitT defines the interface for function
 *          pointer to emit a signal, when client site logging process is done.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef void
(*Log_emitter_callback_emitT)(void);


/**
 * @details Log_emitter_callback_waitT defines the interface for function
 *          pointer to wait for a signal.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef void
(*Log_emitter_callback_waitT)(void);


/**
 * @details Log_emitter_callback_t contain interface functions.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef struct
{
    Log_emitter_callback_emitT client_emit;
    Log_emitter_callback_waitT client_wait;
} Log_emitter_callback_t;


/**
 * @details %Log_emitter_callback_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   client_wait:    function pointer to client wait callback function
 * @param   client_emit:    function pointer to client emit callback function
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitterCallback
*/
bool
Log_emitter_callback_ctor(Log_emitter_callback_t* self,
                          Log_emitter_callback_waitT client_wait,
                          Log_emitter_callback_emitT client_emit);


/**
 * @details %Log_emitter_callback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerEmitterCallback
*/
void
Log_emitter_callback_dtor(Log_emitter_callback_t* self);

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log emitter flow control interface.
 */
#pragma once

/**
 * @defgroup    log_emitter_callback Log emitter flow control interface
 *
 * @brief       This layer provides functions for log emitter flow control.
 * @details     The functions of this level serve to control the flow of log
 *              emitters.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     seos_logger_client
*/
#include "log_filter.h"
#include <stdarg.h>
#include <stdbool.h>

/**
 * @details Log_emitter_callback_emitT defines the interface for function
 *          pointer to emit a signal, when client site logging process is done.
 *
 * @ingroup log_emitter_callback
*/
typedef void
(*Log_emitter_callback_emitT)(void);


/**
 * @details Log_emitter_callback_waitT defines the interface for function
 *          pointer to wait for a signal.
 *
 * @ingroup log_emitter_callback
*/
typedef void
(*Log_emitter_callback_waitT)(void);


/**
 * @details Log_emitter_callback_t contain interface functions.
 *
 * @ingroup log_emitter_callback
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
 * @ingroup log_emitter_callback
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
 * @ingroup log_emitter_callback
*/
void
Log_emitter_callback_dtor(Log_emitter_callback_t* self);

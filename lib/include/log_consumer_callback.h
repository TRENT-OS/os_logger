/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log consumer flow control interface.
 */
#pragma once

/**
 * @defgroup    log_consumer_callback Log consumer flow control interface
 *
 * @brief       This layer provides functions for log consumer flow control.
 * @details     The functions of this level serve to control the flow of log
 *              consumers.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     log_consumer
*/
#include <stdint.h>
#include <stdbool.h>


/**
 * @details Log_consumer_callback_emitT defines the interface for the function
 *          pointer to emit a signal, when server site logging process is done.
 *
 * @ingroup log_consumer_callback
*/
typedef void
(*Log_consumer_callback_emitT)(void);


/**
 * @details Log_consumer_callback_get_sender_id defines the interface for the
 *          function pointer to get sender id by calling RPC function.
 *
 * @return  sender id
 *
 * @ingroup log_consumer_callback
*/
typedef uint32_t
(*Log_consumer_callback_get_sender_id)(void);


/**
 * @details Log_consumer_callback_get_timestampT defines the interface for the
 *          function pointer to get timestamp.
 *
 * @return  timestamp
 *
 * @ingroup log_consumer_callback
*/
typedef uint64_t
(*Log_consumer_callback_get_timestampT)(void);


/**
 * @details Log_consumer_callback_t contain interface functions.
 *
 * @ingroup log_consumer_callback
*/
typedef struct
{
    Log_consumer_callback_emitT          server_emit;
    Log_consumer_callback_get_sender_id  get_sender_id;
    Log_consumer_callback_get_timestampT get_timestamp;
} Log_consumer_callback_t;


/**
 * @details %Log_consumer_dtor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   server_emit:    function pointer to emit callback function
 * @param   get_sender_id:  function pointer to get sender id callback function
 * @param   get_timestamp:  function pointer to get timestamp callback function
 *
 * @return  a status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has occurred
 *
 * @ingroup log_consumer_callback
*/
bool
Log_consumer_callback_ctor(Log_consumer_callback_t* self,
                           Log_consumer_callback_emitT server_emit,
                           Log_consumer_callback_get_sender_id get_sender_id,
                           Log_consumer_callback_get_timestampT get_timestamp);


/**
 * @details %Log_consumer_callback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_consumer_callback
*/
void
Log_consumer_callback_dtor(Log_consumer_callback_t* self);

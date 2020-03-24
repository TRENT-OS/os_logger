/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log consumer flow control interface.
 */
#pragma once

/**
 * @defgroup    OS_LoggerConsumer Log consumer implementation
 *
 * @brief       This layer provides a implementation of concrete log consumer.
 * @details     On the server side, a log consumer is the object that receives
 *              log messages from a client.
 *
 *              A separate log consumer object must be created for each client.
 *              Each log consumer is given a unique name and an id.
 *              The id is assigned to the client in "*.camkes" main file.
 *              Using this ID, the client can be clearly identified when the log
 *              function is called.
 *              \n \n
 *              A log consumer receives his message by calling an RPC function.
 *              When the log is completely processed, the log consumer sends a
 *              signal that tells the clients that further logs can be received.
 *
 *              The RPC interface is blocked for other clients at this time for
 *              processing the log.
 *              \n \n
 *              Before processing the first message, the log server copy on each
 *              given data buffer his server log level.
 *              \n \n
 *              Only one log consumer can be operated at the same time.
 *              The log consumer object is the central object that offers
 *              interfaces for:
 *              \code
 *                  log filter
 *                  log backend
 *                  log file
 *                  flow control functions
 *              \endcode
 *
 *              The latter is the class \link log_consumer_callback \endlink .
 *
 * @see         log_consumer_callback
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_LoggerListT.h"
#include "OS_LoggerFilter.h"
#include "OS_LoggerConsumerCallback.h"
#include "OS_LoggerSubject.h"
#include "OS_LoggerDataBuffer.h"


/**
 * @details Log_consumer_t defines the class datatype.
 *
 * @ingroup OS_LoggerConsumer
*/
typedef struct Log_consumer_t Log_consumer_t;


/**
 * @details Log_consumer_dtorT defines the interface for the function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
typedef void
(*Log_consumer_dtorT)(Log_consumer_t* self);


/**
 * @details Log_consumer_processT defines the interface for function pointer to
 *          process the logging call.
 *
 * @param   self:   pointer to the class
 *
 * @return  an error code
 *
 * @ingroup OS_LoggerConsumer
*/
typedef bool
(*Log_consumer_processT)(Log_consumer_t* self);


/**
 * @details Log_consumer_emitT defines the interface for function pointer to
 *          emit a signal, that the interface is ready for new logs.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
typedef void
(*Log_consumer_emitT)(Log_consumer_t* self);


/**
 * @details Log_consumer_get_timestampT defines the interface for function
 *          pointer to get a timestamp from timeserver.
 *
 * @param   self:   pointer to the class
 *
 * @return  timestamp
 *
 * @ingroup OS_LoggerConsumer
*/
typedef uint64_t
(*Log_consumer_get_timestampT)(Log_consumer_t* self);


/**
 * @details Log_consumer_Vtable contain the member functions to his class.
 *
 * @ingroup OS_LoggerConsumer
*/
typedef struct
{
    Log_consumer_dtorT          dtor;
    Log_consumer_processT       process;
    Log_consumer_emitT          emit;
    Log_consumer_get_timestampT get_timestamp;
} Log_consumer_Vtable;


/**
 * @details Log_consumer_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerConsumer
*/
struct Log_consumer_t
{
    NodeT_t                   node;
    void*                      buf;
    uint32_t                  id;
    Log_info_t                log_info;
    Log_filter_t*              log_filter;
    Log_subject_t*             log_subject;
    void*                      log_file;
    Log_consumer_callback_t*   callback_vtable;
    const Log_consumer_Vtable* vtable;
};


/**
 * @details %Log_consumer_dtor is the constructor.
 *          \n
 *          The parameters "log_filter", "log_file" and "name" are optional.
 *          "log_file" can be NULL, if no log file is needed.
 *          "log_filter" can be NULL, if no log filter is installed.
 *          \n
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @param   self:               pointer to the class
 * @param   buffer:             data buffer
 * @param   log_filter:         layer for log filter (optional)
 * @param   callback_vtable:    layer for callback vtable
 * @param   log_subject:        layer for log subject
 * @param   log_file:           layer for file logging (optional)
 * @param   id:                 log consumer id
 * @param   name:               name (optional)
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerConsumer
*/
bool
Log_consumer_ctor(Log_consumer_t* self,
                  void* buffer,
                  Log_filter_t* log_filter,
                  Log_consumer_callback_t* callback_vtable,
                  Log_subject_t* log_subject,
                  void* log_file,
                  uint32_t id,
                  const char* name);


/**
 * @details %Log_consumer_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
void
Log_consumer_dtor(Log_consumer_t* self);

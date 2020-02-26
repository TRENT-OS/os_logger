/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log filter implementation.
 */
#pragma once

/**
 * @defgroup    log_filter Log filter implementation
 *
 * @brief       This layer provides the interface and implementation of log
 *              filter.
 *
 * @details     The log filter layer is being used by client and server
 *              components.
 *
 *              The log levels depends on the calling specific log function.
 *
 *              A client site log filter serves as flow control. So if the log
 *              level is lower than the installed log filter level than log
 *              messages will be discarded and will not transmitted to the log
 *              server.
 *              \n \n
 *              A server site log filter serves as flow control. So if the log
 *              level lower than the installed log filter level than log
 *              messages will be discarded and will not processed to the log
 *              server backend.
 *              \n \n
 *              If no log filter is installed by client or server, than all
 *              messages will be transmitted or processed to the log server
 *              backend.
 *
 * @ingroup     seos_logger_server, seos_logger_client
*/
#include <stdbool.h>
#include <stdint.h>

/**
 * @details Log_filter_t defines the class datatype.
 *
 * @ingroup log_filter
*/
typedef struct Log_filter_t Log_filter_t;


/**
 * @details Log_filter_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_filter
*/
typedef void
(*Log_filter_dtorT)(Log_filter_t* self);


/**
 * @details Log_filter_filteringT defines the interface for function pointer to
 *          filter logs by log level id.
 *
 * @param   self:       pointer to the class
 * @param   log_level:  log level id
 *
 * @return  an error code
 *
 * @ingroup log_filter
*/
typedef bool
(*Log_filter_filteringT)(Log_filter_t* self, uint8_t log_level);


/**
 * @details Log_filter_Vtable contain the member functions to his class.
 *
 * @ingroup log_filter
*/
typedef struct
{
    Log_filter_dtorT      dtor;      /**< function pointer to destructor */
    Log_filter_filteringT filtering; /**< function pointer to filter function */
} Log_filter_Vtable;


/**
 * @details Log_filter_t contain information about log filter.
 *
 * @ingroup log_filter
*/
struct Log_filter_t
{
    uint8_t                 log_level; /**< log level id */
    const Log_filter_Vtable* vtable;   /**< vtable */
} ;


/**
 * @details %Log_filter_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   log_level:  log level id
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_filter
*/
bool
Log_filter_ctor(Log_filter_t* self, uint8_t log_level);


/**
 * @details %Log_filter_dtor is the destructor.
 *
 * @param   self:       pointer to the class
 *
 * @ingroup log_filter
*/
void
Log_filter_dtor(Log_filter_t* self);

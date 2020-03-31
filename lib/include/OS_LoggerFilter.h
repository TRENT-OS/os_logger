/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log filter implementation.
 */
#pragma once

/**
 * @defgroup    OS_LoggerFilter Log filter implementation
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
 * @ingroup     OS_LoggerServer, OS_LoggerClient
*/
#include <stdbool.h>
#include <stdint.h>

/**
 * @details OS_LoggerFilter_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerFilter
*/
typedef struct OS_LoggerFilter_Handle OS_LoggerFilter_Handle_t;


/**
 * @details OS_LoggerFilter_dtor_t defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerFilter
*/
typedef void
(*OS_LoggerFilter_dtor_t)(OS_LoggerFilter_Handle_t* self);


/**
 * @details OS_LoggerFilter_isFilteredOut_t defines the interface for function
 *          pointer to filter logs by log level id.
 *
 * @param   self:       pointer to the class
 * @param   log_level:  log level id
 *
 * @retval  true  - if filtered out
 * @retval  false - if NOT filtered out
 *
 * @ingroup OS_LoggerFilter
*/
typedef bool
(*OS_LoggerFilter_isFilteredOut_t)(
    OS_LoggerFilter_Handle_t* self,
    uint8_t log_level);


/**
 * @details OS_LoggerFilter_vtable_t contain the member functions to his class.
 *
 * @ingroup OS_LoggerFilter
*/
typedef struct
{
    OS_LoggerFilter_dtor_t          dtor;
    OS_LoggerFilter_isFilteredOut_t isFilteredOut;
} OS_LoggerFilter_vtable_t;


/**
 * @details OS_LoggerFilter_Handle contain information about log filter.
 *
 * @ingroup OS_LoggerFilter
*/
struct OS_LoggerFilter_Handle
{
    uint8_t                         log_level; /**< log level id */
    const OS_LoggerFilter_vtable_t* vtable;    /**< vtable */
} ;


/**
 * @details %OS_LoggerFilter_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   log_level:  log level id
 *
 * @return  an status code
 *
 * @retval  true  - if allright
 * @retval  false - if an error has occurred
 *
 * @ingroup OS_LoggerFilter
*/
bool
OS_LoggerFilter_ctor(OS_LoggerFilter_Handle_t* self, uint8_t log_level);


/**
 * @details %OS_LoggerFilter_dtor is the destructor.
 *
 * @param   self:       pointer to the class
 *
 * @ingroup OS_LoggerFilter
*/
void
OS_LoggerFilter_dtor(OS_LoggerFilter_Handle_t* self);

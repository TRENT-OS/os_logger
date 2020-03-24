/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server backend class definition
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutput Log server backend interface
 *
 * @brief       Defined an generally class for log server backend
 *              implementation.
 *
 * @details     This is an definition of a class interface for concrete log
 *              server backend implementations.
 *              \n
 *              A "Log_output_t" object provides the interface to a log server
 *              backend and at the same time is this object an observer.
 *
 *              Several observer can be assigned to one subject. To avoid the
 *              "malloc" function, objects of this type are defined externally.
 *
 *              To ensure that the observers are assigned to their subjects,
 *              they are assigned based on their nodes.
 *
 *              The auxiliary object ListT_t is also responsible for managing
 *              this nodes.
 *              \n
 *              A log message should be printed on the log server backend.
 *              The handling of the format of the log is defined in the
 *              "Log_format_t" level.
 *
 *              This class is the consumer of the log formats and uses the log
 *              format only.
 *
 * @ingroup     OS_LoggerAbstractOutput
*/
#include "OS_LoggerAbstractOutput.h"
#include "OS_LoggerFormat.h"
#include "OS_LoggerListT.h"


/**
 * @details Log_output_t contain the vtable to his class.
 *          This layer is depend on the layers \link NodeT_t \endlink ,
 *          \link ListT_t \endlink and \link Log_format_t \endlink.
 *          \n
 *          The "Log_format_t" layer is implemented as an pointer, so the
 *          concrete implementation depends on this class itself.
 *
 *          The log format is are interchangeable due to its special
 *          implementation.
 *
 * @ingroup OS_LoggerOutput
*/
typedef struct
{
    NodeT_t             node;        /**< node interface */
    ListT_t             listT;       /**< helper functions for nodes */
    Log_format_t*        log_format; /**< layer for log format */
    const Output_Vtable* vtable;     /**< pointer to vtable */
} Log_output_t;

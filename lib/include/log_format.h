/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log format implementation.
 */
#pragma once

/**
 * @defgroup    log_format Log format implementation
 *
 * @brief       Derived class from class \link abstract_format \endlink and
 *              provides an implemention for log format.
 *
 * @details     This level provides an implementation for converting and
 *              printing log format.
 *
 *              This class is derived by abstract format.
 *              \n
 *              The print function is a helper function to print the temporary
 *              log format from "Log_format_t->buffer" via "printf().
 *              \n
 *              Before print the log message to an backend, convert the log
 *              message in the defined format.
 *
 * @ingroup     abstract_format
*/
#include "abstract_format.h"

/**
 * @brief   Length of the timestamp string
 *
 *          Timestamp has the following format ' dd.mm.yyyy-hh:mm:ss'.
 *
 * @note    Altough null terminator is not included in the calculation the value
 *          includes blank space in front to separate it from the previous
 *          field.
 */
#define FORMAT_TIMESTAMP_LENGTH         20

#define FORMAT_BUFFER_SIZE              (LOG_ID_AND_NAME_LENGTH  +          \
                                         FORMAT_TIMESTAMP_LENGTH +          \
                                         LOG_LEVEL_SERVER_LENGTH +          \
                                         LOG_LEVEL_CLIENT_LENGTH +          \
                                         LOG_MESSAGE_LENGTH)

/**
 * @details Log_format_t contain informations about the log format and a pointer
 *          the parent vtable.
 *
 * @ingroup log_format
*/
typedef struct
{
    char
    buffer[FORMAT_BUFFER_SIZE]; /**< buffer for log format message */

    const FormatT_Vtable*
    vtable;                     /**< pointer to parent vtable */
} Log_format_t;


/**
 * @details %Log_format_ctor is the constructor.
 *
 * @param   self:   pointer to the class
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup log_format
*/
bool
Log_format_ctor(Log_format_t* self);

/**
 * @details %Log_format_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_format
*/
void
Log_format_dtor(Format_t* self);

/**
 * @brief Prints data in the given format.
 * @param self:   pointer to the class
 */
void
Log_format_print(Format_t* self);


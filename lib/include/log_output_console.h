/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server console backend implementation
 */
#pragma once

/**
 * @defgroup    log_output_console Log server console backend implementation
 *
 * @brief       Derived class from class \link abstract_log_output \endlink and
 *              provides an implemention for log server console backend.
 *
 * @details     This level provides an implementation for console spending.
 *              \n
 *              It uses as class interface "Log_output_t".
 *              \n
 *              For sending logs to console uses function "printf()".
 *              \n \n
 *              This layer implements the observer functions.
 *
 * @ingroup     log_output
*/


#pragma once


#include "log_output.h"


/**
 * @details %Log_output_console_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   log_format: pointer to log format layer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup log_output_console
*/
bool
Log_output_console_ctor(Log_output_t* self,
                        Log_format_t* log_format);


/**
 * @details %Log_output_console_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_output_console
*/
void
Log_output_console_dtor(Output_t* self);
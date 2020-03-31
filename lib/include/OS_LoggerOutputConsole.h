/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server console backend implementation
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutputConsole Log server console backend
 *
 * @brief       Derived class from class \link abstract_log_output \endlink and
 *              provides an implemention for log server console backend.
 *
 * @details     This level provides an implementation for console spending.
 *              \n
 *              It uses as class interface "OS_LoggerOutput_Handle_t".
 *              \n
 *              For sending logs to console uses function "printf()".
 *              \n \n
 *              This layer implements the observer functions.
 *
 * @ingroup     OS_LoggerOutput
*/
#include "OS_LoggerOutput.h"


/**
 * @details %OS_LoggerOutputConsole_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   log_format: pointer to log format layer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup OS_LoggerOutputConsole
*/
bool
OS_LoggerOutputConsole_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* log_format);


/**
 * @details %OS_LoggerOutputConsole_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerOutputConsole
*/
void
OS_LoggerOutputConsole_dtor(OS_LoggerAbstractOutput_Handle_t* self);

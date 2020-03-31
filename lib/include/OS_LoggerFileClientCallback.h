/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log file client flow control.
 */
#pragma once

/**
 * @defgroup    OS_LoggerFileClientCallback Log file client flow control
 *
 * @brief       This layer provides functions for log file client flow control.
 *
 * @details     The functions of this level serve to control the flow of log
 *              file client.
 *
 *              This layer only defines the interface functions.
 *
 *              The implementation itself is not place here.
 *
 * @ingroup     OS_LoggerFileClient
*/
#include "OS_LoggerSymbols.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerFileClientCallback_read_t defines the interface for
 *          function pointer to read a log file from log server.
 *
 * @param   filename:       name of log file
 * @param   offset:         offset in log file
 * @param   len:            length of buffer
 * @param   log_file_size:  size of log file in bytes
 *
 * @return  count read bytes
 *
 * @ingroup OS_LoggerFileClient
*/
typedef int64_t
(*OS_LoggerFileClientCallback_read_t)(
    const char* filename,
    uint64_t    offset,
    uint64_t    len,
    int64_t*    log_file_size);


/**
 * @details OS_LoggerFileClientCallback_Handle_t contain interface functions.
 *
 * @ingroup OS_LoggerFileClient
*/
typedef struct
{
    OS_LoggerFileClientCallback_read_t read_log_file;
} OS_LoggerFileClientCallback_Handle_t;


/**
 * @details %OS_LoggerFileClientCallback_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   read_log_file:  function pointer to read log file function
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerFileClient
*/
bool
OS_LoggerFileClientCallback_ctor(
    OS_LoggerFileClientCallback_Handle_t* self,
    OS_LoggerFileClientCallback_read_t read_log_file);


/**
 * @details %OS_LoggerFileClientCallback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerFileClient
*/
void
OS_LoggerFileClientCallback_dtor(OS_LoggerFileClientCallback_Handle_t* self);

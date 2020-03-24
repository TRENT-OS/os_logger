/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log file client flow control.
 */
#pragma once

/**
 * @defgroup    log_file_client_callback Log file client flow control
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
 * @ingroup     log_file_client
*/
#include "OS_LoggerSymbols.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details Log_file_client_callback_read_log_fileT defines the interface for
 *          function pointer to read a log file from log server.
 *
 * @param   filename:       name of log file
 * @param   offset:         offset in log file
 * @param   len:            length of buffer
 * @param   log_file_size:  size of log file in bytes
 *
 * @return  count read bytes
 *
 * @ingroup log_file_client_callback
*/
typedef int64_t
(*Log_file_client_callback_read_log_fileT)(
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t* log_file_size);


/**
 * @details Log_file_client_callback_t contain interface functions.
 *
 * @ingroup log_file_client_callback
*/
typedef struct
{
    Log_file_client_callback_read_log_fileT read_log_file;
} Log_file_client_callback_t;


/**
 * @details %Log_file_client_callback_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   read_log_file:  function pointer to read log file function
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_file_client_callback
*/
bool
Log_file_client_callback_ctor(
    Log_file_client_callback_t* self,
    Log_file_client_callback_read_log_fileT read_log_file);


/**
 * @details %Log_file_client_callback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_file_client_callback
*/
void
Log_file_client_callback_dtor(Log_file_client_callback_t* self);

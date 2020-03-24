/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log file client implementation.
 */
#pragma once

/**
 * @defgroup    log_file_client Log file client implementation
 *
 * @brief       This layer provides the implementation of log file client
 *              functions.
 *
 * @details     This is a client library, so this library doesn't know anything
 *              about the file handling on the log server i.e. it doesn't
 *              matter, which filesystem and so on is installed on server site.
 *              \n \n
 *              This layer is a consumer layer and provides client site
 *              functionality to read content from a log file or a complete log
 *              file.
 *
 * @ingroup     seos_logger_client
*/
#include "OS_LoggerSymbols.h"
#include "OS_LoggerFileClientCallback.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details Log_file_client_t defines the class datatype.
 *
 * @ingroup log_file_client
*/
typedef struct Log_file_client_t Log_file_client_t;


/**
 * @details Log_file_client_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_file_client
*/
typedef void
(*Log_file_client_dtorT)(Log_file_client_t* self);


/**
 * @details Log_file_client_read_log_fileT defines the interface for function
 *          pointer to read a log file from log server.
 *
 *          The parameter "len" is the buffer size, i.e. how many bytes should
 *          be read from the log file and written to the buffer when the
 *          function is called.
 *
 *          This size is particularly important because the max. buffer size of
 *          CamkES is 4096 bytes.
 *
 * @param   self:       pointer to the class
 * @param   filename:   name of log file
 * @param   offset:     offset in log file
 * @param   len:        length of buffer
 *
 * @return  an error code
 *
 * @ingroup log_file_client
*/
typedef bool
(*Log_file_client_read_log_fileT)(
    Log_file_client_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len);


/**
 * @details Log_file_client_Vtable contain the member functions to his class.
 *
 * @ingroup log_file_client
*/
typedef struct
{
    Log_file_client_dtorT          dtor;
    Log_file_client_read_log_fileT read_log_file;
} Log_file_client_Vtable;


/**
 * @details Log_file_client_t contain the vtable to his class.
 *
 * @ingroup log_file_client
*/
struct Log_file_client_t
{
    void* src_buf;
    void* dest_buf;
    Log_file_client_callback_t* callback_vtable;
    const Log_file_client_Vtable* vtable;
};


/**
 * @details %Log_file_client_ctor is the constructor. \n
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @param   self:                       pointer to the class
 * @param   src_buf:                    pointer to source data buffer
 * @param   dest_buf:                   pointer to destination data buffer
 * @param   log_file_client_callback:   layer for callback vtable
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_file_client
*/
bool
Log_file_client_ctor(
    Log_file_client_t* self,
    void* src_buf,
    void* dest_buf,
    Log_file_client_callback_t* log_file_client_callback);


/**
 * @details %Log_file_client_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_file_client
*/
void
Log_file_client_dtor(Log_file_client_t* self);


/**
 * @details %Log_file_client_read_log_file provides to read a log file from log
 *          server.
 *
 * @param   self:       pointer to the class
 * @param   filename:   name of log file
 * @param   offset:     offset in log file
 * @param   len:        buffer size, i.e. how many bytes should be read from the
 *                      log file and written to the buffer when the function is
 *                      called. This size is particularly important because the
 *                      max. buffer size of camkes is 4096 bytes.
 *
 * @return  an error code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_file_client
*/
bool
Log_file_client_read_log_file(Log_file_client_t* self,
                              const char* filename,
                              uint64_t offset,
                              uint64_t len);

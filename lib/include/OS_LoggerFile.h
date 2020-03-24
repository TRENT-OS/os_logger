/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log file interface and implementation.
 */
#pragma once

/**
 * @defgroup    log_file Log file interface and implementation
 *
 * @brief       This layer provides the interface and implementation of log
 *              file.
 *
 * @details     This layer is a server site implementation. It needs a
 *              filesystem partition manager backend.
 *
 *              Functions from this layer are only necessary, if log messages
 *              shall be store in a log file. Dealing with log files needs a
 *              underlying file system.
 *
 *              So, this layer has an dependency to the filesystem backend and
 *              includes the "seos_fs.h" header file.
 *              \n \n
 *              As an interface layer provides this layer functions to create
 *              and read log files on filesystem.
 *
 * @ingroup     seos_logger_server
*/
#include "OS_LoggerSymbols.h"
#include "seos_fs.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details Log_file_t defines the class datatype.
 *
 * @ingroup log_file
*/
typedef struct Log_file_t Log_file_t;


/**
 * @details Log_file_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_file
*/
typedef void
(*Log_file_dtorT)(Log_file_t* self);


/**
 * @details Log_file_create_log_fileT defines the interface for function pointer
 *          to create a log file in filesystem backend.
 *
 * @param   self:   pointer to the class
 *
 * @return  an error code
 *
 * @ingroup log_file
*/
typedef bool
(*Log_file_create_log_fileT)(Log_file_t* self);


/**
 * @details Log_file_read_log_fileT defines the interface for function pointer
 *          to read a log file from filesystem backend.
 *
 *          The parameter "len" is the buffer size, i.e. how many bytes should
 *          be read from the log file and written to the buffer when the
 *          function is called.
 *
 *          This size is particularly important because the max. buffer size of
 *          CAmkES is 4096 bytes.
 *
 * @param   self:           pointer to the class
 * @param   filename:       name of log file
 * @param   offset:         offset in log file
 * @param   len:            length of buffer
 * @param   log_file_size:  size of log file in bytes
 *
 * @return  count read bytes
 *
 * @ingroup log_file
*/
typedef int64_t
(*Log_file_read_log_fileT)(Log_file_t* self, const char* filename,
                           uint64_t offset, uint64_t len, int64_t log_file_size);


/**
 * @details Log_file_get_consumer_by_filenameT defines the interface for
 *          function pointer to get a pointer of log consumer by log filename.
 *
 * @param   filename:   name of log file
 *
 * @return  pointer to log consumer
 *
 * @ingroup log_file
*/
typedef void*
(*Log_file_get_consumer_by_filenameT)(const char* filename);


/**
 * @details Log_file_Vtable contain the member functions to his class.
 *
 * @ingroup log_file
*/
typedef struct
{
    Log_file_dtorT                     dtor;
    Log_file_create_log_fileT          create_log_file;
    Log_file_read_log_fileT            read_log_file;
    Log_file_get_consumer_by_filenameT get_consumer_by_filename;
} Log_file_Vtable;


/**
 * @details Log_file_info_t contain information about log file and filesystem
 *          backend.
 *
 * @ingroup log_file
*/
typedef struct
{
    uint8_t      drv_id;                            /**< partition id */
    char         filename[LOG_ID_AND_NAME_LENGTH];  /**< filename */
    hPartition_t phandle;                           /**< partition handle */
    uint64_t     offset;                            /**< offset in log file */
    uint64_t     lenght;                            /**< size of log file */
} Log_file_info_t;


/**
 * @details Log_file_t contain the vtable to his class.
 *
 * @ingroup log_file
*/
struct Log_file_t
{
    Log_file_info_t       log_file_info; /**< struct Log_file_info_t */
    const Log_file_Vtable* vtable;       /**< vtable */
};


/**
 * @details %Log_file_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   drv_id:     partition id
 * @param   filename:   name of log file
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_file
*/
bool
Log_file_ctor(Log_file_t* self,
              uint8_t drv_id,
              const char* filename);


/**
 * @details %Log_file_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_file
*/
void
Log_file_dtor(Log_file_t* self);


/**
 * @details %Log_file_create_log_file provides to create a log file.
 *
 * @param   self:   pointer to the class
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_file
*/
bool
Log_file_create_log_file(Log_file_t* self);

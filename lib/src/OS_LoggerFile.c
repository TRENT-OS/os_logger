/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "Logger/Server/OS_LoggerFile.h"
#include "Logger/Server/OS_LoggerConsumerChain.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

// forward declaration
static int64_t _Log_file_read_log_file(
    OS_LoggerFile_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t log_file_size);

static void* _Log_file_get_consumer_by_filename(const char* filename);

static const OS_LoggerFile_vtable_t Log_file_vtable =
{
    .dtor                     = OS_LoggerFile_dtor,
    .create_log_file          = OS_LoggerFile_create,
    .read_log_file            = _Log_file_read_log_file,
    .get_consumer_by_filename = _Log_file_get_consumer_by_filename
};



static void*
_Log_file_get_consumer_by_filename(const char* filename)
{
    if (filename == NULL)
    {
        return NULL;
    }

    OS_LoggerConsumerChain_Handle_t* consumer_chain;
    OS_LoggerConsumer_Handle_t* log_consumer;

    consumer_chain = OS_LoggerConsumerChain_getInstance();

    if (consumer_chain == NULL)
    {
        return NULL;
    }

    if (consumer_chain->node.first == NULL)
    {
        return NULL;
    }

    log_consumer = consumer_chain->node.first;

    do
    {
        if (log_consumer->log_file == NULL)
        {
            continue;
        }

        if (0 == strcmp(
                ((OS_LoggerFile_Handle_t*)
                 log_consumer->log_file)->log_file_info.filename,
                filename))
        {
            return (void*)log_consumer;
            break;
        }
    }
    while ( (log_consumer = OS_LoggerListT_getNext(
                                &log_consumer->node)) != NULL );

    return NULL;
}



int64_t
API_LOG_SERVER_READ_LOG_FILE(
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t* log_file_size)
{
    if (filename == NULL)
    {
        return -1;
    }

    OS_LoggerConsumer_Handle_t* log_consumer =
        OS_LoggerConsumerChain_getSender();

    if (log_consumer == NULL)
    {
        return -1;
    }

    OS_LoggerConsumer_Handle_t* log_consumer_filename =
        (OS_LoggerConsumer_Handle_t*)_Log_file_get_consumer_by_filename(
            filename);

    if (log_consumer_filename == NULL)
    {
        return -1;
    }

    off_t sz;
    OS_LoggerFile_Handle_t* logFile = (OS_LoggerFile_Handle_t*)
                                      log_consumer_filename->log_file;
    OS_Error_t err = OS_FileSystemFile_getSize(logFile->log_file_info.hFs,
                                               filename, &sz);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to get size of file: %s\n",
               __func__,
               filename);
        return -1;
    }

    *log_file_size = sz;

    logFile->log_file_info.length = (uint64_t) (*log_file_size);
    if (offset > sz)
    {
        printf(
            "%s(): ERROR offset %"PRIu64" greater file size %" PRIiMAX " for: %s\n",
            __func__,
            offset,
            sz,
            filename);

        return -1;
    }

    // Handling signed offset overflow
    if ((INT64_MAX < (offset + len)) || ((offset + len) < offset))
    {
        printf(
            "%s(): Offset overflow detected. offset = %"PRIu64", len = %"PRIu64"\n",
            __func__,
            offset,
            len);

        return -1;
    }

    if (*log_file_size <= (int64_t)(offset + len))
    {
        len = (uint64_t)((uint64_t)(*log_file_size) - offset);
    }

    OS_FileSystemFile_Handle_t hFile;
    err = OS_FileSystemFile_open(logFile->log_file_info.hFs,
                                 &hFile,
                                 filename,
                                 OS_FileSystem_OpenMode_RDONLY,
                                 OS_FileSystem_OpenFlags_NONE);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to open file: %s\n", __func__, filename);
        return -1;
    }

    err = OS_FileSystemFile_read(logFile->log_file_info.hFs,
                                 hFile,
                                 (size_t)offset,
                                 (size_t)len,
                                 log_consumer->entry);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to read file: %s\n", __func__, filename);
        return -1;
    }

    err = OS_FileSystemFile_close(logFile->log_file_info.hFs,
                                  hFile);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to close file: %s\n", __func__, filename);
        return -1;
    }

    return (int64_t)len;
}



OS_Error_t
OS_LoggerFile_ctor(
    OS_LoggerFile_Handle_t* self,
    OS_FileSystem_Handle_t hFs,
    const char* filename)
{
    OS_Logger_CHECK_SELF(self);

    if (NULL == filename
        || (strlen(filename) >= sizeof(self->log_file_info.filename)))
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->vtable = &Log_file_vtable;

    self->log_file_info.hFs = hFs;
    strcpy(self->log_file_info.filename, filename);

    return OS_SUCCESS;
}



void
OS_LoggerFile_dtor(OS_LoggerFile_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof(OS_LoggerFile_Handle_t));
}



OS_Error_t
OS_LoggerFile_create(OS_LoggerFile_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    // create empty file
    OS_FileSystemFile_Handle_t hFile;

    OS_Error_t err = OS_FileSystemFile_open(self->log_file_info.hFs,
                                            &hFile,
                                            self->log_file_info.filename,
                                            OS_FileSystem_OpenMode_RDWR,
                                            OS_FileSystem_OpenFlags_CREATE);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to open file: %s\n",
               __func__,
               self->log_file_info.filename);
        return OS_ERROR_INVALID_HANDLE;
    }

    err = OS_FileSystemFile_close(self->log_file_info.hFs,
                                  hFile);
    if (OS_SUCCESS != err)
    {
        printf("%s(): ERROR: failed to close file: %s\n",
               __func__,
               self->log_file_info.filename);
        return err;
    }

    self->log_file_info.offset = 0;

    return OS_SUCCESS;
}



static int64_t
_Log_file_read_log_file(
    OS_LoggerFile_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t log_file_size)
{
    OS_Logger_CHECK_SELF(self);

    return API_LOG_SERVER_READ_LOG_FILE(filename, offset, len, &log_file_size);
}

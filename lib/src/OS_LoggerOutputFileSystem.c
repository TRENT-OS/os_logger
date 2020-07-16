/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputFileSystem.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include "Logger/Server/OS_LoggerFile.h"
#include <string.h>
#include <stdio.h>

static
OS_Error_t
update(
    OS_LoggerOutput_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    if (data == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    OS_FileSystemFile_Handle_t hFile;
    OS_LoggerConsumer_Handle_t* log_consumer =
        (OS_LoggerConsumer_Handle_t*)data;

    // check if log_file is installed
    if (log_consumer->log_file == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    // log format layer
    self->logFormat->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)self->logFormat,
        ((OS_LoggerConsumer_Handle_t*)data)->entry);


    OS_LoggerFile_Handle_t* logFile = (OS_LoggerFile_Handle_t*)
                                      log_consumer->log_file;

    OS_Error_t err = OS_FileSystemFile_open(logFile->log_file_info.hFs,
                                            &hFile,
                                            logFile->log_file_info.filename,
                                            OS_FileSystem_OpenMode_WRONLY,
                                            OS_FileSystem_OpenFlags_NONE);
    if (OS_SUCCESS != err)
    {
        printf("Fail to open file: %s!\n", logFile->log_file_info.filename);
        return OS_ERROR_INVALID_HANDLE;
    }

    err = OS_FileSystemFile_write(logFile->log_file_info.hFs,
                                  hFile,
                                  logFile->log_file_info.offset,
                                  (size_t)strlen(self->logFormat->buffer),
                                  self->logFormat->buffer);
    if (OS_SUCCESS != err)
    {
        printf("Fail to write file: %s!\n", logFile->log_file_info.filename);
        return err;
    }

    err = OS_FileSystemFile_close(logFile->log_file_info.hFs, hFile);
    if (OS_SUCCESS != err)
    {
        printf("Fail to close file: %s!\n", logFile->log_file_info.filename);
        return err;
    }

    logFile->log_file_info.offset += strlen(self->logFormat->buffer);

    return OS_SUCCESS;
}

OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* logFormat)
{
    return OS_LoggerOutput_ctor(self, logFormat, update);
}

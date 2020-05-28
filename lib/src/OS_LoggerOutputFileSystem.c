/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputFileSystem.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include "Logger/Server/OS_LoggerFile.h"
#include "OS_Filesystem.h"
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

    hFile_t fhandle;
    OS_LoggerConsumer_Handle_t* log_consumer =
        (OS_LoggerConsumer_Handle_t*)data;

    // check if log_file is installed
    if (log_consumer->log_file == NULL)
    {
        // Debug_printf
        return OS_ERROR_INVALID_PARAMETER;
    }

    // log format layer
    self->logFormat->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)self->logFormat,
        ((OS_LoggerConsumer_Handle_t*)data)->entry);

    fhandle = OS_Filesystem_openFile(
                  ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.phandle,
                  ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename,
                  FA_WRITE);

    if (!OS_Filesystem_validateFileHandle(fhandle))
    {
        printf(
            "Fail to open file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return OS_ERROR_INVALID_HANDLE;
    }

    OS_Error_t result = OS_Filesystem_writeFile(
                            fhandle,
                            (long)((OS_LoggerFile_Handle_t*)log_consumer->log_file)
                            ->log_file_info.offset,
                            (long)strlen(self->logFormat->buffer),
                            self->logFormat->buffer);

    if (OS_SUCCESS != result)
    {
        printf(
            "Fail to write file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return result;
    }

    result = OS_Filesystem_closeFile(fhandle);
    if (OS_SUCCESS != result)
    {
        printf(
            "Fail to close file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return result;
    }

    ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.offset
    += strlen(self->logFormat->buffer);

    return OS_SUCCESS;
}

OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* logFormat)
{
    return OS_LoggerOutput_ctor(self, logFormat, update);
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputFileSystem.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include "Logger/Server/OS_LoggerFile.h"
#include "OS_Filesystem.h"
#include <string.h>
#include <stdio.h>

static OS_Error_t _Log_output_filesystem_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data);

static OS_Error_t _Log_output_filesystem_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data);

static const OS_LoggerAbstractOutput_vtable_t Log_output_filesystem_vtable =
{
    .parent.update = _Log_output_filesystem_update,
    .print         = _Log_output_filesystem_print
};

OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* log_format)
{
    OS_Logger_CHECK_SELF(self);

    if (log_format == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->node.prev = NULL;
    self->node.next = NULL;

    self->log_format = log_format;

    self->vtable = &Log_output_filesystem_vtable;

    return OS_SUCCESS;
}

static
OS_Error_t
_Log_output_filesystem_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    if (data == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;
    log_output->vtable->print(
        (OS_LoggerAbstractOutput_Handle_t*)log_output,
        data);

    return OS_SUCCESS;
}



static
OS_Error_t
_Log_output_filesystem_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    if (data == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    hFile_t fhandle;
    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;
    OS_LoggerConsumer_Handle_t* log_consumer =
        (OS_LoggerConsumer_Handle_t*)data;

    // check if log_file is installed
    if (log_consumer->log_file == NULL)
    {
        // Debug_printf
        return OS_ERROR_INVALID_PARAMETER;
    }

    // log format layer
    log_output->log_format->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format,
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
                            (long)strlen(log_output->log_format->buffer),
                            log_output->log_format->buffer);

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
    += strlen(log_output->log_format->buffer);

    return OS_SUCCESS;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputFileSystem.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include "Logger/Server/OS_LoggerFile.h"
#include "OS_Filesystem.h"
#include <string.h>
#include <stdio.h>



// forward declaration
static void _Log_observer_dtor(OS_LoggerAbstractObserver_Handle_t* self);

static OS_Error_t _Log_output_filesystem_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data);

static OS_Error_t _Log_output_filesystem_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data);

static const OS_LoggerAbstractOutput_vtable_t Log_output_filesystem_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_filesystem_update,
    .dtor          = OS_LoggerOutputFileSystem_dtor,
    .print         = _Log_output_filesystem_print
};



static void
_Log_observer_dtor(OS_LoggerAbstractObserver_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractObserver_Handle_t));
}



OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* log_format)
{
    OS_Logger_CHECK_SELF(self);

    if (log_format == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    OS_LoggerListT_ctor(&self->listT);

    self->node.prev = NULL;
    self->node.next = NULL;

    self->log_format = log_format;

    self->vtable = &Log_output_filesystem_vtable;

    return SEOS_SUCCESS;
}



void
OS_LoggerOutputFileSystem_dtor(OS_LoggerAbstractOutput_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerOutput_Handle_t));
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
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;
    log_output->vtable->print(
        (OS_LoggerAbstractOutput_Handle_t*)log_output,
        data);

    return SEOS_SUCCESS;
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
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    hFile_t fhandle;
    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;
    OS_LoggerConsumer_Handle_t* log_consumer =
        (OS_LoggerConsumer_Handle_t*)data;

    // check if log_file is installed
    if (log_consumer->log_file == NULL)
    {
        // Debug_printf
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    // log format layer
    log_output->log_format->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format,
        &log_consumer->log_info);

    fhandle = OS_Filesystem_openFile(
                  ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.phandle,
                  ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename,
                  FA_WRITE);

    if (!OS_Filesystem_validateFileHandle(fhandle))
    {
        printf(
            "Fail to open file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return SEOS_ERROR_INVALID_HANDLE;
    }

    OS_Error_t result = OS_Filesystem_writeFile(
                            fhandle,
                            (long)((OS_LoggerFile_Handle_t*)log_consumer->log_file)
                            ->log_file_info.offset,
                            (long)strlen(log_output->log_format->buffer),
                            log_output->log_format->buffer);

    if (SEOS_SUCCESS != result)
    {
        printf(
            "Fail to write file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return result;
    }

    result = OS_Filesystem_closeFile(fhandle);
    if (SEOS_SUCCESS != result)
    {
        printf(
            "Fail to close file: %s!\n",
            ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.filename);

        return result;
    }

    ((OS_LoggerFile_Handle_t*)log_consumer->log_file)->log_file_info.offset
    += strlen(log_output->log_format->buffer);

    return SEOS_SUCCESS;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputConsole.h"
#include "Logger/Server/OS_LoggerConsumer.h"

static OS_Error_t _Log_output_console_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data);

static OS_Error_t _Log_output_console_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data);

static const OS_LoggerAbstractOutput_vtable_t Log_output_console_vtable =
{
    .parent.update = _Log_output_console_update,
    .print         = _Log_output_console_print
};

OS_Error_t
OS_LoggerOutputConsole_ctor(
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

    self->vtable = &Log_output_console_vtable;

    return OS_SUCCESS;
}

static
OS_Error_t
_Log_output_console_update(
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



static OS_Error_t
_Log_output_console_print(OS_LoggerAbstractOutput_Handle_t* self, void* data)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;

    // log format layer
    log_output->log_format->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format,
        ((OS_LoggerConsumer_Handle_t*)data)->entry);

    log_output->log_format->vtable->print(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format);

    return OS_SUCCESS;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputConsole.h"
#include "Logger/Server/OS_LoggerConsumer.h"



// forward declaration
static void _Log_observer_dtor(OS_LoggerAbstractObserver_Handle_t* self);

static seos_err_t _Log_output_console_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data);

static seos_err_t _Log_output_console_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data);

static const OS_LoggerAbstractOutput_vtable_t Log_output_console_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_console_update,
    .dtor          = OS_LoggerOutputConsole_dtor,
    .print         = _Log_output_console_print
};



static void
_Log_observer_dtor(OS_LoggerAbstractObserver_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractObserver_Handle_t));
}



seos_err_t
OS_LoggerOutputConsole_ctor(
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

    self->vtable = &Log_output_console_vtable;

    return SEOS_SUCCESS;
}



void
OS_LoggerOutputConsole_dtor(OS_LoggerAbstractOutput_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerOutput_Handle_t));
}



static
seos_err_t
_Log_output_console_update(
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



static seos_err_t
_Log_output_console_print(OS_LoggerAbstractOutput_Handle_t* self, void* data)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerOutput_Handle_t* log_output = (OS_LoggerOutput_Handle_t*)self;

    // log format layer
    log_output->log_format->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format,
        (OS_LoggerDataBuffer_info*)
        & (((OS_LoggerConsumer_Handle_t*)data)->log_info));

    log_output->log_format->vtable->print(
        (OS_LoggerAbstractFormat_Handle_t*)log_output->log_format);

    return SEOS_SUCCESS;
}

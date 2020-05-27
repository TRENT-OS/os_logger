/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerFormat.h"
#include "Logger/Server/OS_LoggerTimestamp.h"
#include <stdio.h>

// forward declaration
static OS_Error_t _Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerDataBuffer_info* log_info);

static const OS_LoggerAbstractFormat_vtable_t Log_format_vtable =
{
    .dtor    = OS_LoggerFormat_dtor,
    .convert = _Log_format_convert,
    .print   = OS_LoggerFormat_print
};

void
OS_LoggerFormat_ctor(OS_LoggerFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable = &Log_format_vtable;
}

void
OS_LoggerFormat_dtor(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerFormat_Handle_t));
}

static OS_Error_t
_Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerDataBuffer_info* log_info)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerFormat_Handle_t* log_format;
    char* buf;
    OS_LoggerTime_Handle_t tm;

    if (log_info == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    log_format = (OS_LoggerFormat_Handle_t*)self;
    buf = log_format->buffer;

    OS_LoggerTimestamp_getTime(&log_info->timestamp, 0, &tm);

    unsigned long msg_len = strlen(log_info->log_databuffer.log_message);

    if (msg_len > OS_Logger_MESSAGE_LENGTH)
    {
        msg_len = OS_Logger_MESSAGE_LENGTH;
    }

    sprintf(
        buf,
        "%-*s %02d.%02d.%04d-%02d:%02d:%02d %*u %*u %.*s\n",
        OS_Logger_ID_AND_NAME_LENGTH, log_info->log_id_and_name,
        tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec,
        OS_Logger_LOG_LEVEL_LENGTH,
        log_info->log_databuffer.log_level_srv,
        OS_Logger_LOG_LEVEL_LENGTH,
        log_info->log_databuffer.log_level_client,
        (int)msg_len, log_info->log_databuffer.log_message);

    return OS_SUCCESS;
}

void
OS_LoggerFormat_print(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerFormat_Handle_t* log_format = (OS_LoggerFormat_Handle_t*)self;
    char* buf = log_format->buffer;

    printf("%s", buf);
}

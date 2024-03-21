/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "Logger/Server/OS_LoggerFormat.h"
#include "Logger/Server/OS_LoggerTimestamp.h"
#include <stdio.h>

// forward declaration
static OS_Error_t _Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerEntry_t const* const entry);

static const OS_LoggerAbstractFormat_vtable_t Log_format_vtable =
{
    .convert = _Log_format_convert,
    .print   = OS_LoggerFormat_print
};

void
OS_LoggerFormat_ctor(OS_LoggerFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable = &Log_format_vtable;
}

static OS_Error_t
_Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerEntry_t const* const entry)
{
    OS_Logger_CHECK_SELF(self);

    if (NULL == entry)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    OS_LoggerTimestamp_Handle_t* const timestamp =
        OS_LoggerTimestamp_getInstance();

    timestamp->timestamp = entry->consumerMetadata.timestamp;

    OS_LoggerTime_Handle_t tm;
    OS_LoggerTimestamp_getTime(timestamp, 0, &tm);

    size_t msg_len = strlen(entry->msg);

    if (msg_len > OS_Logger_ENTRY_MESSAGE_LENGTH)
    {
        msg_len = OS_Logger_ENTRY_MESSAGE_LENGTH;
    }

    OS_LoggerFormat_Handle_t* const log_format =
        (OS_LoggerFormat_Handle_t*)self;

    sprintf(
        log_format->buffer,
        "%.*u %-*s %02d.%02d.%04d-%02d:%02d:%02d %*u %*u %.*s\n",

        OS_Logger_ID_LENGTH, entry->consumerMetadata.id,
        OS_Logger_NAME_LENGTH, entry->consumerMetadata.name,

        tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec,

        OS_Logger_LOG_LEVEL_LENGTH, entry->emitterMetadata.filteringLevel,
        OS_Logger_LOG_LEVEL_LENGTH, entry->consumerMetadata.filteringLevel,

        (int)msg_len, entry->msg);

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

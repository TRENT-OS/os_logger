/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "log_format.h"
#include "timestamp.h"
#include <stdio.h>

// forward declaration
static bool _Log_format_convert(Format_t* self, Log_info_t* log_info);

static const FormatT_Vtable Log_format_vtable =
{
    .dtor    = Log_format_dtor,
    .convert = _Log_format_convert,
    .print   = Log_format_print
};

bool
Log_format_ctor(Log_format_t* self)
{
    CHECK_SELF(self);

    self->vtable = &Log_format_vtable;

    return true;
}

void
Log_format_dtor(Format_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_format_t));
}

static bool
_Log_format_convert(Format_t* self, Log_info_t* log_info)
{
    CHECK_SELF(self);

    Log_format_t* log_format;
    char* buf;
    Time_t tm;

    if (log_info == NULL)
    {
        // Debug_printf
        return false;
    }

    log_format = (Log_format_t*)self;
    buf = log_format->buffer;

    Timestamp_get_time(&log_info->timestamp, 0, &tm);

    unsigned long msg_len = strlen(log_info->log_databuffer.log_message);

    if (msg_len > LOG_MESSAGE_LENGTH)
    {
        msg_len = LOG_MESSAGE_LENGTH;
    }

    sprintf(
        buf,
        "%-*s %02d.%02d.%04d-%02d:%02d:%02d %*d%*d %.*s\n",
        LOG_ID_AND_NAME_LENGTH, log_info->log_id_and_name,
        tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec,
        FORMAT_LOG_LEVEL_SERVER_LENGTH,
        log_info->log_databuffer.log_level_srv,
        FORMAT_LOG_LEVEL_CLIENT_LENGTH,
        log_info->log_databuffer.log_level_client,
        (int)msg_len, log_info->log_databuffer.log_message);

    return true;
}

void
Log_format_print(Format_t* self)
{
    CHECK_SELF(self);

    Log_format_t* log_format = (Log_format_t*)self;
    char* buf = log_format->buffer;

    printf("%s", buf);
}

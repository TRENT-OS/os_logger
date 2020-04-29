/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Client/OS_LoggerEmitter.h"
#include "Logger/Common/OS_LoggerDataBuffer.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>
#include <stdio.h>

typedef void  (*OS_LoggerEmitter_dtor_t)(void);
typedef void* (*OS_LoggerEmitter_getBuffer_t)(void);

struct OS_LoggerEmitter_Handle
{
    void*                        buf;
    OS_LoggerFilter_Handle_t*    log_filter;
    event_notify_func_t          emit;
};

// Singleton
static OS_LoggerEmitter_Handle_t _log_emitter;
static OS_LoggerEmitter_Handle_t* this = NULL;

OS_LoggerEmitter_Handle_t*
OS_LoggerEmitter_getInstance(
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter,
    event_notify_func_t       emit)
{
    if (sizeof (buffer) > DATABUFFER_SIZE)
    {
        // Debug_printf
        return NULL;
    }

    if (emit == NULL)
    {
        // Debug_printf
        return NULL;
    }

    if (this == NULL)
    {
        this = &_log_emitter;
        this->buf = buffer;
        this->emit = emit;
    }

    this->log_filter = log_filter;

    return this;
}

void
OS_LoggerEmitter_dtor(void)
{
    memset(this, 0, sizeof (OS_LoggerEmitter_Handle_t));
    this = NULL;
}

bool
OS_LoggerEmitter_log(uint8_t log_level, const char* format, ...)
{
    if (NULL == this)
    {
        // Debug_printf
        return false;
    }

    if (format == NULL)
    {
        // Debug_printf
        return false;
    }

    int retval = false;
    char buf[OS_Logger_MESSAGE_LENGTH];

    if (this->log_filter != NULL)
    {
        if (this->log_filter->vtable->isFilteredOut(
                this->log_filter,
                log_level))
        {
            // Debug_printf -> Log filter!!!
            return false;
        }
    }

    va_list args;
    va_start (args, format);

    if (strlen(format) > OS_Logger_MESSAGE_LENGTH)
    {
        // Debug_printf
        return false;
    }

    retval = vsnprintf(buf, OS_Logger_MESSAGE_LENGTH, format, args);
    if (retval < 0 || retval > OS_Logger_MESSAGE_LENGTH)
    {
        return false;
    }

    OS_LoggerDataBuffer_setClientLogLevel(this->buf, log_level);
    OS_LoggerDataBuffer_setLogMessage(this->buf, buf);

    va_end (args);

    this->emit();

    return true;
}

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

OS_Error_t
OS_LoggerEmitter_log(uint8_t log_level, const char* format, ...)
{
    if (NULL == this)
    {
        return SEOS_ERROR_INVALID_HANDLE;
    }

    if (NULL == format)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    char buf[OS_Logger_MESSAGE_LENGTH];

    if (this->log_filter != NULL)
    {
        if (this->log_filter->vtable->isFilteredOut(
                this->log_filter,
                log_level))
        {
            return SEOS_SUCCESS;
        }
    }

    if (strlen(format) > OS_Logger_MESSAGE_LENGTH)
    {
        return SEOS_ERROR_GENERIC;
    }

    va_list args;
    va_start (args, format);

    const int retval = vsnprintf(buf, OS_Logger_MESSAGE_LENGTH, format, args);

    va_end (args);

    if (retval < 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    if (retval > OS_Logger_MESSAGE_LENGTH)
    {
        return SEOS_ERROR_BUFFER_TOO_SMALL;
    }

    OS_LoggerDataBuffer_setClientLogLevel(this->buf, log_level);

    const OS_Error_t seosResult = OS_LoggerDataBuffer_setLogMessage(
                                      this->buf,
                                      buf);

    if (SEOS_SUCCESS != seosResult)
    {
        return seosResult;
    }

    this->emit();

    return SEOS_SUCCESS;
}

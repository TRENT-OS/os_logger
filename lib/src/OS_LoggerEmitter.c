/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Client/OS_LoggerEmitter.h"
#include "Logger/Common/OS_LoggerEntry.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>
#include <stdio.h>

typedef void* (*OS_LoggerEmitter_getBuffer_t)(void);

struct OS_LoggerEmitter_Handle
{
    OS_LoggerEntry_t*            entry;
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
        return NULL;
    }

    if (emit == NULL)
    {
        return NULL;
    }

    if (this == NULL)
    {
        this = &_log_emitter;
        this->entry = (OS_LoggerEntry_t*)buffer;
        this->emit = emit;
    }

    this->log_filter = log_filter;

    return this;
}

OS_Error_t
OS_LoggerEmitter_log(uint8_t logLevel, const char* format, ...)
{
    if (NULL == this)
    {
        return OS_ERROR_INVALID_HANDLE;
    }

    if (NULL == format)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (this->log_filter != NULL)
    {
        this->entry->emitterMetadata.filteringLevel
            = this->log_filter->log_level;

        if (this->log_filter->vtable->isFilteredOut(
                this->log_filter,
                logLevel))
        {
            return OS_SUCCESS;
        }
    }
    else
    {
        this->entry->emitterMetadata.filteringLevel = 0U;
    }


    this->entry->emitterMetadata.level = logLevel;

    if (strlen(format) > OS_Logger_MESSAGE_LENGTH)
    {
        return OS_ERROR_GENERIC;
    }

    va_list args;
    va_start (args, format);

    const int retval = vsnprintf(
                           this->entry->msg,
                           sizeof(this->entry->msg),
                           format,
                           args);

    va_end (args);

    if (retval < 0)
    {
        return OS_ERROR_GENERIC;
    }

    if (retval > OS_Logger_MESSAGE_LENGTH)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    this->emit();

    return OS_SUCCESS;
}

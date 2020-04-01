/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Client/OS_LoggerEmitter.h"
#include "Logger/Common/OS_LoggerDataBuffer.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>
#include <stdio.h>

typedef void  (*OS_LoggerEmitter_dtor_t)(void);
typedef void* (*OS_LoggerEmitter_getBuffer_t)(void);
typedef bool  (*OS_LoggerEmitter_wait_t)(void);
typedef bool  (*OS_LoggerEmitter_emit_t)(void);

typedef bool
(*OS_LoggerEmitter_log_t)(uint8_t log_level, const char* format, ...);

typedef struct
{
    OS_LoggerEmitter_dtor_t      dtor;
    OS_LoggerEmitter_getBuffer_t get_buffer;
    OS_LoggerEmitter_wait_t      wait;
    OS_LoggerEmitter_emit_t      emit;
    OS_LoggerEmitter_log_t       log;
} OS_LoggerEmitter_vtable_t;

struct OS_LoggerEmitter_Handle
{
    void*                                buf;
    OS_LoggerFilter_Handle_t*            log_filter;
    OS_LoggerEmitterCallback_Handle_t*   callback_vtable;
    const OS_LoggerEmitter_vtable_t*     vtable;
};

// forward declaration
static void* _Log_emitter_get_buffer(void);
static bool  _Log_emitter_wait(void);
static bool  _Log_emitter_emit(void);

static const OS_LoggerEmitter_vtable_t Log_emitter_vtable =
{
    .dtor            = OS_LoggerEmitter_dtor,
    .get_buffer      = _Log_emitter_get_buffer,
    .wait            = _Log_emitter_wait,
    .emit            = _Log_emitter_emit,
    .log             = OS_LoggerEmitter_log,
};



// Singleton
static OS_LoggerEmitter_Handle_t _log_emitter;
static OS_LoggerEmitter_Handle_t* this = NULL;



OS_LoggerEmitter_Handle_t*
OS_LoggerEmitter_getInstance(
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter,
    OS_LoggerEmitterCallback_Handle_t* callback_vtable)
{
    if (sizeof (buffer) > DATABUFFER_SIZE)
    {
        // Debug_printf
        return NULL;
    }

    if (callback_vtable == NULL)
    {
        // Debug_printf
        return NULL;
    }

    if (this == NULL)
    {
        this = &_log_emitter;
        this->buf = buffer;

        this->vtable = &Log_emitter_vtable;
        this->callback_vtable = callback_vtable;
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



static void*
_Log_emitter_get_buffer(void)
{
    OS_Logger_CHECK_SELF(this);

    return this->buf;
}



static bool
_Log_emitter_wait(void)
{
    OS_Logger_CHECK_SELF(this);

    if (this->callback_vtable->client_wait != NULL)
    {
        this->callback_vtable->client_wait();
    }

    return true;
}



static bool
_Log_emitter_emit(void)
{
    OS_Logger_CHECK_SELF(this);

    if (this->callback_vtable->client_emit != NULL)
    {
        this->callback_vtable->client_emit();
    }

    return true;
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

    this->vtable->wait();

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

    OS_LoggerDataBuffer_setClientLogLevel(this->vtable->get_buffer(), log_level);
    OS_LoggerDataBuffer_setLogMessage(this->vtable->get_buffer(), buf);

    va_end (args);

    this->vtable->emit();

    return true;
}

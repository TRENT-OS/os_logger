/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerEmitterCallback.h"
#include "OS_LoggerSymbols.h"
#include <string.h>



bool
OS_LoggerEmitterCallback_ctor(
    OS_LoggerEmitterCallback_Handle_t* self,
    OS_LoggerEmitterCallback_wait_t client_wait,
    OS_LoggerEmitterCallback_emit_t client_emit)
{
    OS_Logger_CHECK_SELF(self);

    // "client_wait" can be NULL, if OS_LoggerEmitter_Handle_t will be declared as "local" log server
    if (/*client_wait == NULL ||*/ client_emit == NULL)
    {
        // Debug_printf
        return false;
    }

    self->client_emit = client_emit;
    self->client_wait = client_wait;

    return true;
}



void
OS_LoggerEmitterCallback_dtor(OS_LoggerEmitterCallback_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerEmitterCallback_Handle_t));
}

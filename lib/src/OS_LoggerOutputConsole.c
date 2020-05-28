/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerOutputConsole.h"
#include "Logger/Server/OS_LoggerConsumer.h"

static OS_Error_t
update(OS_LoggerOutput_Handle_t* self, void* data)
{
    OS_Logger_CHECK_SELF(self);

    self->logFormat->vtable->convert(
        (OS_LoggerAbstractFormat_Handle_t*)self->logFormat,
        ((OS_LoggerConsumer_Handle_t*)data)->entry);

    self->logFormat->vtable->print(
        (OS_LoggerAbstractFormat_Handle_t*)self->logFormat);

    return OS_SUCCESS;
}

OS_Error_t
OS_LoggerOutputConsole_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* logFormat)
{
    return OS_LoggerOutput_ctor(self, logFormat, &update);
}

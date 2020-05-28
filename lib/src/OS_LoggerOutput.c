/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "Logger/Server/OS_LoggerOutput.h"

OS_Error_t
OS_LoggerOutput_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* logFormat,
    OS_LoggerOutput_update_t update)
{
    OS_Logger_CHECK_SELF(self);

    if (logFormat == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->node.prev = NULL;
    self->node.next = NULL;

    self->logFormat = logFormat;

    self->update = update;

    return OS_SUCCESS;
}

OS_Error_t
OS_LoggerOutput_update(
    OS_LoggerOutput_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    return self->update(self, data);
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "Logger/Server/OS_LoggerAbstractFormat.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <stdbool.h>
#include <string.h>

void
FormatT_dtor(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractFormat_Handle_t));
}

OS_Error_t
FormatT_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerDataBuffer_info* log_info)
{
    OS_Logger_CHECK_SELF(self);

    if (log_info == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    return self->vtable->convert(self, log_info);
}

void
FormatT_print(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable->print(self);
}

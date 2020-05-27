/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "Logger/Server/OS_LoggerAbstractFormat.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <stdbool.h>
#include <string.h>

OS_Error_t
FormatT_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerEntry_t const* const entry);
{
    OS_Logger_CHECK_SELF(self);

    if (log_info == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    return self->vtable->convert(self, entry);
}

void
FormatT_print(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable->print(self);
}

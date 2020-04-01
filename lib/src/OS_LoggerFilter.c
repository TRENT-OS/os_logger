/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Common/OS_LoggerFilter.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>



// forward declaration
static bool _isFilteredOut_t(
    OS_LoggerFilter_Handle_t* self,
    uint8_t log_level);



static const OS_LoggerFilter_vtable_t Log_filter_vtable =
{
    .dtor      = OS_LoggerFilter_dtor,
    .isFilteredOut = _isFilteredOut_t
};



bool
OS_LoggerFilter_ctor(OS_LoggerFilter_Handle_t* self, uint8_t log_level)
{
    OS_Logger_CHECK_SELF(self);

    self->log_level = log_level;
    self->vtable = &Log_filter_vtable;

    return true;
}



void
OS_LoggerFilter_dtor(OS_LoggerFilter_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerFilter_Handle_t));
}



static bool
_isFilteredOut_t(OS_LoggerFilter_Handle_t* self, uint8_t log_level)
{
    // if self == NULL, return value is true...
    // ...because no log filter is installed
    if (self == NULL)
    {
        return false;
    }

    if (self->log_level < log_level)
    {
        return true;
    }

    return false;
}

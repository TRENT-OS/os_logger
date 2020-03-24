/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerFilter.h"
#include "OS_LoggerSymbols.h"
#include <string.h>



// forward declaration
static bool _Log_filter_filtering(Log_filter_t* self, uint8_t log_level);



static const Log_filter_Vtable Log_filter_vtable =
{
    .dtor      = Log_filter_dtor,
    .filtering = _Log_filter_filtering
};



bool
Log_filter_ctor(Log_filter_t* self, uint8_t log_level)
{
    CHECK_SELF(self);

    self->log_level = log_level;
    self->vtable = &Log_filter_vtable;

    return true;
}



void
Log_filter_dtor(Log_filter_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_filter_t));
}



static bool
_Log_filter_filtering(Log_filter_t* self, uint8_t log_level)
{
    // if self == NULL, return value is true...
    // ...because no log filter is installed
    if (self == NULL)
    {
        return true;
    }

    if (self->log_level < log_level)
    {
        return false;
    }

    return true;
}

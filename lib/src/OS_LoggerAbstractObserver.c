/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

#include "Logger/Server/OS_LoggerAbstractObserver.h"
#include <string.h>

void
OS_LoggerAbstractObserver_dtor(OS_LoggerAbstractObserver_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractObserver_Handle_t));
}

OS_Error_t
OS_LoggerAbstractObserver_update(
    OS_LoggerAbstractObserver_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    return self->vtable->update(self, data);
}

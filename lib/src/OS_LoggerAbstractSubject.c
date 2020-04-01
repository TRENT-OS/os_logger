/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerAbstractSubject.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>

void
OS_LoggerAbstractSubject_dtor(OS_LoggerAbstractSubject_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractSubject_Handle_t));
}

bool
OS_LoggerAbstractSubject_attach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    return self->vtable->attach(self, observer);
}

bool
OS_LoggerAbstractSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    return self->vtable->detach(self, observer);
}

void
OS_LoggerAbstractSubject_notify(
    OS_LoggerAbstractSubject_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable->notify(self, data);
}

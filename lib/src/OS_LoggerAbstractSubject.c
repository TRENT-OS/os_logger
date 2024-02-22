/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "Logger/Server/OS_LoggerAbstractSubject.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>

OS_Error_t
OS_LoggerAbstractSubject_attach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    if (observer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    return self->vtable->attach(self, observer);
}

OS_Error_t
OS_LoggerAbstractSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    if (observer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
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

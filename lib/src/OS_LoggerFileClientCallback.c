/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "Logger/Client/OS_LoggerFileClientCallback.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>



OS_Error_t
OS_LoggerFileClientCallback_ctor(
    OS_LoggerFileClientCallback_Handle_t* self,
    OS_LoggerFileClientCallback_read_t read_log_file)
{
    OS_Logger_CHECK_SELF(self);

    if (read_log_file == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->read_log_file = read_log_file;

    return OS_SUCCESS;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Client/OS_LoggerFileClientCallback.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>



seos_err_t
OS_LoggerFileClientCallback_ctor(
    OS_LoggerFileClientCallback_Handle_t* self,
    OS_LoggerFileClientCallback_read_t read_log_file)
{
    OS_Logger_CHECK_SELF(self);

    if (read_log_file == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    self->read_log_file = read_log_file;

    return SEOS_SUCCESS;
}



void
OS_LoggerFileClientCallback_dtor(OS_LoggerFileClientCallback_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerFileClientCallback_Handle_t));
}

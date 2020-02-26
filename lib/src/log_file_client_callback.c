/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "log_file_client_callback.h"
#include <string.h>



bool
Log_file_client_callback_ctor(Log_file_client_callback_t* self,
                              Log_file_client_callback_read_log_fileT read_log_file)
{
    CHECK_SELF(self);

    if (read_log_file == NULL)
    {
        // Debug_printf
        return false;
    }

    self->read_log_file = read_log_file;

    return true;
}



void
Log_file_client_callback_dtor(Log_file_client_callback_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_file_client_callback_t));
}

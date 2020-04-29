/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerConsumerCallback.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>

bool
OS_LoggerConsumerCallback_ctor(
    OS_LoggerConsumerCallback_t* self,
    OS_LoggerConsumerCallback_getSenderId_t get_sender_id,
    OS_LoggerConsumerCallback_getTimestamp_t get_timestamp)
{
    OS_Logger_CHECK_SELF(self);

    // "get_timestamp" can be NULL, if timestamp is not necessary
    if (get_sender_id == NULL)
    {
        // Debug_printf
        return false;
    }

    self->get_sender_id = get_sender_id;
    self->get_timestamp = get_timestamp;

    return true;
}



void
OS_LoggerConsumerCallback_dtor(OS_LoggerConsumerCallback_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerConsumerCallback_t));
}

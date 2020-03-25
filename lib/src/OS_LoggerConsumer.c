/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerConsumer.h"
#include <string.h>
#include <stdio.h>

// forward declaration
static bool _Log_consumer_process(OS_LoggerConsumer_Handle_t* self);
static uint64_t _Log_consumer_get_timestamp(OS_LoggerConsumer_Handle_t* self);

static
bool _Log_consumer_init(
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter);

static bool _create_id_string(
    OS_LoggerConsumer_Handle_t* self,
    uint32_t id,
    const char* name);

static void _Log_consumer_emit(OS_LoggerConsumer_Handle_t* self);



static const OS_LoggerConsumer_vtable_t Log_consumer_vtable =
{
    .dtor          = OS_LoggerConsumer_dtor,
    .process       = _Log_consumer_process,
    .emit          = _Log_consumer_emit,
    .get_timestamp = _Log_consumer_get_timestamp,
};



static bool
_Log_consumer_init(void* buffer, OS_LoggerFilter_Handle_t* log_filter)
{
    if (buffer == NULL)
    {
        return false;
    }

    OS_LoggerDataBuffer_clear(buffer);

    if (log_filter != NULL)
    {
        // Debug_printf -> no log filter installed
        OS_LoggerDataBuffer_setServerLogLevel(buffer, log_filter->log_level);
    }

    return true;
}



static
bool
_create_id_string(
    OS_LoggerConsumer_Handle_t* self,
    uint32_t id,
    const char* name)
{
    if (self == NULL)
    {
        return false;
    }

    sprintf(self->log_info.log_id_and_name, "%.*u", OS_Logger_ID_LENGTH, id);

    if (name != NULL)
    {
        sprintf(self->log_info.log_id_and_name + OS_Logger_ID_LENGTH, " %.*s",
                OS_Logger_NAME_LENGTH - 1, // Leaving space for the NULL terminator
                name);
    }
    else
    {
        sprintf(self->log_info.log_id_and_name + OS_Logger_ID_LENGTH, " %.*s",
                OS_Logger_NAME_LENGTH - 1, // Leaving space for the NULL terminator
                "");
    }


    return true;
}



bool
OS_LoggerConsumer_ctor(
    OS_LoggerConsumer_Handle_t* self,
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter,
    OS_LoggerConsumerCallback_t* callback_vtable,
    OS_LoggerSubject_Handle_t* log_subject,
    void* log_file,
    uint32_t id,
    const char* name)
{
    OS_Logger_CHECK_SELF(self);

    bool retval = false;

    // "log_file" can be NULL, if no log file is needed
    // "log_filter" can be NULL, if no log filter is installed
    if (buffer == NULL || callback_vtable == NULL
        || log_subject == NULL /*|| log_file == NULL || log_filter == NULL*/)
    {
        // Debug_printf
        return retval;
    }

    if (sizeof (buffer) > DATABUFFER_SIZE)
    {
        // Debug_printf
        return retval;
    }

    self->buf = buffer;
    self->id = id;
    self->log_filter = log_filter;
    self->log_subject = log_subject;
    self->log_file = log_file;

    self->vtable = &Log_consumer_vtable;
    self->callback_vtable = callback_vtable;

    retval = _create_id_string(self, id, name);
    if (retval == false)
    {
        // Debug_printf
        return false;
    }

    retval = _Log_consumer_init(self->buf, self->log_filter);

    return retval;
}



void
OS_LoggerConsumer_dtor(OS_LoggerConsumer_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerConsumer_Handle_t));
}



static void
_Log_consumer_emit(OS_LoggerConsumer_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    if (self->callback_vtable->server_emit != NULL)
    {
        self->callback_vtable->server_emit();
    }
}



static uint64_t
_Log_consumer_get_timestamp(OS_LoggerConsumer_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    if (self->callback_vtable->get_timestamp != NULL)
    {
        return self->callback_vtable->get_timestamp();
    }

    return 0;
}



static bool
_Log_consumer_process(OS_LoggerConsumer_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    // get log level client
    OS_LoggerDataBuffer_getClientLogLevel(self->buf,
                                          &self->log_info.log_databuffer);

    if (self->log_filter != NULL)
    {
        if (self->log_filter->vtable->filtering(self->log_filter,
                                                self->log_info.log_databuffer.log_level_client) == false)
        {
            // Debug_printf -> Log filter!!!
            OS_LoggerDataBuffer_clear(self->buf);
            return false;
        }
    }

    OS_LoggerDataBuffer_getInfo(self->buf, &self->log_info.log_databuffer);

    OS_LoggerDataBuffer_clear(self->buf);

    self->log_info.timestamp.timestamp = self->vtable->get_timestamp(self);

    // log subject
    self->log_subject->vtable->notify(
        (OS_LoggerAbstractSubject_Handle_t*) self->log_subject,
        (void*)self);

    return true;
}

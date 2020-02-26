/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "log_consumer.h"
#include <string.h>
#include <stdio.h>



#if !defined(EOF)
#define EOF     -1
#endif



// forward declaration
static bool     _Log_consumer_process(Log_consumer_t* self);
static uint64_t _Log_consumer_get_timestamp(Log_consumer_t* self);
static bool     _Log_consumer_init(void* buffer, Log_filter_t* log_filter);
static bool     _create_id_string(Log_consumer_t* self, uint32_t id,
                                  const char* name);
static void     _Log_consumer_emit(Log_consumer_t* self);



static const Log_consumer_Vtable Log_consumer_vtable =
{
    .dtor          = Log_consumer_dtor,
    .process       = _Log_consumer_process,
    .emit          = _Log_consumer_emit,
    .get_timestamp = _Log_consumer_get_timestamp,
};



static bool
_Log_consumer_init(void* buffer, Log_filter_t* log_filter)
{
    if (buffer == NULL)
    {
        return false;
    }

    Log_databuffer_clear_databuffer(buffer);

    if (log_filter != NULL)
    {
        // Debug_printf -> no log filter installed
        Log_databuffer_set_log_level_server(buffer, log_filter->log_level);
    }

    return true;
}



static bool
_create_id_string(Log_consumer_t* self, uint32_t id, const char* name)
{
    if (self == NULL)
    {
        return false;
    }

    sprintf(self->log_info.log_id_name, "%06u", id);

    if (name != NULL)
    {
        sprintf(self->log_info.log_id_name + 6, " %s", name);
    }

    self->log_info.log_id_name[strlen(self->log_info.log_id_name)] = '\0';

    return true;
}



bool
Log_consumer_ctor(Log_consumer_t* self,
                  void* buffer,
                  Log_filter_t* log_filter,
                  Log_consumer_callback_t* callback_vtable,
                  Log_subject_t* log_subject,
                  void* log_file,
                  uint32_t id,
                  const char* name)
{
    CHECK_SELF(self);

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
Log_consumer_dtor(Log_consumer_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_consumer_t));
}



static void
_Log_consumer_emit(Log_consumer_t* self)
{
    CHECK_SELF(self);

    if (self->callback_vtable->server_emit != NULL)
    {
        self->callback_vtable->server_emit();
    }
}



static uint64_t
_Log_consumer_get_timestamp(Log_consumer_t* self)
{
    CHECK_SELF(self);

    if (self->callback_vtable->get_timestamp != NULL)
    {
        return self->callback_vtable->get_timestamp();
    }

    return 0;
}



static bool
_Log_consumer_process(Log_consumer_t* self)
{
    CHECK_SELF(self);

    // get log level client
    Log_databuffer_get_log_level_client(self->buf, &self->log_info.log_databuffer);

    if (self->log_filter != NULL)
    {
        if (self->log_filter->vtable->filtering(self->log_filter,
                                                self->log_info.log_databuffer.log_level_client) == false)
        {
            // Debug_printf -> Log filter!!!
            Log_databuffer_clear_databuffer(self->buf);
            return false;
        }
    }

    Log_databuffer_get_info(self->buf, &self->log_info.log_databuffer);

    Log_databuffer_clear_databuffer(self->buf);

    self->log_info.timestamp.timestamp = self->vtable->get_timestamp(self);

    // log subject
    self->log_subject->vtable->notify((Subject_t*)self->log_subject, (void*)self);

    return true;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerConsumer.h"
#include <string.h>
#include <stdio.h>

// forward declaration
static void _Log_consumer_process(OS_LoggerConsumer_Handle_t* self);
static uint64_t _Log_consumer_get_timestamp(OS_LoggerConsumer_Handle_t* self);

static const OS_LoggerConsumer_vtable_t Log_consumer_vtable =
{
    .process       = _Log_consumer_process,
    .get_timestamp = _Log_consumer_get_timestamp,
};

OS_Error_t
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

    // "log_file" can be NULL, if no log file is needed
    // "log_filter" can be NULL, if no log filter is installed
    if (buffer == NULL || callback_vtable == NULL
        || log_subject == NULL /*|| log_file == NULL || log_filter == NULL*/)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->entry = (OS_LoggerEntry_t*)buffer;
    self->log_filter = log_filter;
    self->log_subject = log_subject;
    self->log_file = log_file;

    self->vtable = &Log_consumer_vtable;
    self->callback_vtable = callback_vtable;

    self->entry->consumerMetadata.id = id;

    if (NULL != name)
    {
        snprintf(
            self->entry->consumerMetadata.name,
            OS_Logger_NAME_LENGTH + 1,
            name);
    }
    else
    {
        self->entry->consumerMetadata.name[0] = '\0';
    }

    return OS_SUCCESS;
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

static
void
_Log_consumer_process(OS_LoggerConsumer_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    if (self->log_filter != NULL)
    {
        self->entry->consumerMetadata.filteringLevel = self->log_filter->log_level;

        if (self->log_filter->vtable->isFilteredOut(
                self->log_filter,
                self->entry->emitterMetadata.level))
        {
            return;
        }
    }
    else
    {
        self->entry->consumerMetadata.filteringLevel = 0U;
    }

    self->entry->consumerMetadata.timestamp = self->vtable->get_timestamp(self);

    // log subject
    self->log_subject->vtable->notify(
        (OS_LoggerAbstractSubject_Handle_t*) self->log_subject,
        (void*)self);

    return;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerSubject.h"
#include "Logger/Server/OS_LoggerOutput.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include "Logger/Common/OS_LoggerEntry.h"
#include <string.h>
#include <stddef.h>



static const OS_LoggerAbstractSubject_vtable_t Log_subject_vtable =
{
    .attach = OS_LoggerSubject_attach,
    .detach = OS_LoggerSubject_detach,
    .notify = OS_LoggerSubject_notify
};



void
OS_LoggerSubject_ctor(OS_LoggerSubject_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerSubjectNode_t* node = NULL;

    self->vtable = &Log_subject_vtable;

    node = &self->node;
    node->first = NULL;
}

OS_Error_t
OS_LoggerSubject_attach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerSubject_Handle_t* log_subject;
    OS_LoggerOutput_Handle_t* last = NULL;

    if (observer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    log_subject = (OS_LoggerSubject_Handle_t*)self;
    last = log_subject->node.first;

    // first entry
    if (last == NULL)
    {
        last = observer;
    }
    else
    {
        last = OS_LoggerListT_getLast((OS_LoggerNodeT_Handle_t*)last);
    }

    const bool isInserted = (observer == OS_LoggerListT_insert(
                                 &last->node,
                                 &observer->node));

    log_subject->node.first = OS_LoggerListT_getFirst(&last->node);

    return isInserted ? OS_SUCCESS : OS_ERROR_OPERATION_DENIED;
}

OS_Error_t
OS_LoggerSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerOutput_Handle_t* observer)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerSubject_Handle_t* log_subject;
    OS_LoggerSubjectNode_t* node = NULL;

    if (observer == NULL)
    {
        // Debug_printf
        return OS_ERROR_INVALID_PARAMETER;
    }

    log_subject = (OS_LoggerSubject_Handle_t*)self;
    node = &log_subject->node;

    if (!OS_LoggerListT_hasPrevious(&observer->node))
    {
        node->first = OS_LoggerListT_getNext(&observer->node);
    }

    const bool isDeleted = (observer->node.prev == OS_LoggerListT_erase(
                                &observer->node));

    return isDeleted ? OS_SUCCESS : OS_ERROR_OPERATION_DENIED;
}



void
OS_LoggerSubject_notify(OS_LoggerAbstractSubject_Handle_t* self, void* data)
{
    OS_Logger_CHECK_SELF(self);

    if (data == NULL)
    {
        // Debug_printf
        return;
    }

    OS_LoggerSubject_Handle_t* log_subject = (OS_LoggerSubject_Handle_t*)self;

    for (
        OS_LoggerOutput_Handle_t* observer = log_subject->node.first;
        NULL != observer;
        observer = OS_LoggerListT_getNext(&observer->node)
    )
    {
        OS_LoggerOutput_update(observer, data);
    }
}

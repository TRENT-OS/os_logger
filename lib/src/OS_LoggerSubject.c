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
    OS_LoggerAbstractObserver_Handle_t* observer)
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
        last = (OS_LoggerOutput_Handle_t*)observer;
    }
    else
    {
        last = last->listT.vtable->get_last(log_subject->node.first);
    }

    const bool isInserted =
        ((OS_LoggerOutput_Handle_t*)observer)->listT.vtable->insert(
            &last->node,
            &((OS_LoggerOutput_Handle_t*)observer)->node);

    log_subject->node.first =
        (void*)((OS_LoggerOutput_Handle_t*)observer)->listT.vtable->get_first(
            &last->node);

    return isInserted ? OS_SUCCESS : OS_ERROR_OPERATION_DENIED;
}



OS_Error_t
OS_LoggerSubject_detach(
    OS_LoggerAbstractSubject_Handle_t* self,
    OS_LoggerAbstractObserver_Handle_t* observer)
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

    if (!((OS_LoggerOutput_Handle_t*)observer)->listT.vtable->has_prev( &((
                OS_LoggerOutput_Handle_t*)observer)->node) )
    {
        node->first =
            (void*)((OS_LoggerOutput_Handle_t*)observer)->listT.vtable->get_next(
                &((OS_LoggerOutput_Handle_t*)observer)->node );
    }

    const bool isDeleted =
        ((OS_LoggerOutput_Handle_t*)observer)->listT.vtable->delete ( &((
                    OS_LoggerOutput_Handle_t*)observer)->node );

    return isDeleted ? OS_SUCCESS : OS_ERROR_OPERATION_DENIED;
}



void
OS_LoggerSubject_notify(OS_LoggerAbstractSubject_Handle_t* self, void* data)
{
    OS_Logger_CHECK_SELF(self);

    OS_LoggerSubject_Handle_t* log_subject;
    OS_LoggerOutput_Handle_t* log_output;

    if (data == NULL)
    {
        // Debug_printf
        return;
    }

    log_subject = (OS_LoggerSubject_Handle_t*)self;

    // traverse list
    log_output = log_subject->node.first;

    OS_Logger_CHECK_SELF(log_output);

    do
    {
        log_output->vtable->parent.update(
            (OS_LoggerAbstractObserver_Handle_t*)log_output,
            data);
    }
    while ( (log_output = log_output->listT.vtable->get_next(
                              &log_output->node)) != NULL );
}

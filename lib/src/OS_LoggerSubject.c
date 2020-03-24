/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerSubject.h"
#include "OS_LoggerOutput.h"
#include "OS_LoggerSymbols.h"
#include "OS_LoggerDataBuffer.h"
#include <string.h>
#include <stddef.h>



static const Subject_Vtable Log_subject_vtable =
{
    .dtor   = Log_subject_dtor,
    .attach = Log_subject_attach,
    .detach = Log_subject_detach,
    .notify = Log_subject_notify
};



bool
Log_subject_ctor(Log_subject_t* self)
{
    CHECK_SELF(self);

    bool retval = false;
    Subject_node_t* node = NULL;

    self->vtable = &Log_subject_vtable;

    node = &self->node;
    node->first = NULL;

    return retval;
}



void
Log_subject_dtor(Subject_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_subject_t));
}



bool
Log_subject_attach(Subject_t* self, Observer_t* observer)
{
    CHECK_SELF(self);

    bool isInsert = false;
    Log_subject_t* log_subject;
    Log_output_t* last = NULL;

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    log_subject = (Log_subject_t*)self;
    last = log_subject->node.first;

    // first entry
    if (last == NULL)
    {
        last = (Log_output_t*)observer;
    }
    else
    {
        last = last->listT.vtable->get_last(log_subject->node.first);
    }

    isInsert = ((Log_output_t*)observer)->listT.vtable->insert(&last->node,
                                                               &((Log_output_t*)observer)->node);

    log_subject->node.first = (void*)((Log_output_t*)
                                      observer)->listT.vtable->get_first(&last->node);

    return isInsert;
}



bool
Log_subject_detach(Subject_t* self, Observer_t* observer)
{
    CHECK_SELF(self);

    bool isDelete = false;
    Log_subject_t* log_subject;
    Subject_node_t* node = NULL;

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    log_subject = (Log_subject_t*)self;
    node = &log_subject->node;

    if (!((Log_output_t*)observer)->listT.vtable->has_prev( &((
            Log_output_t*)observer)->node) )
    {
        node->first = (void*)((Log_output_t*)observer)->listT.vtable->get_next( &((
                          Log_output_t*)observer)->node );
    }

    isDelete = ((Log_output_t*)observer)->listT.vtable->delete ( &((
                   Log_output_t*)observer)->node );

    return isDelete;
}



void
Log_subject_notify(Subject_t* self, void* data)
{
    CHECK_SELF(self);

    Log_subject_t* log_subject;
    Log_output_t* log_output;

    if (data == NULL)
    {
        // Debug_printf
        return;
    }

    log_subject = (Log_subject_t*)self;

    // traverse list
    log_output = log_subject->node.first;

    CHECK_SELF(log_output);

    do
    {
        log_output->vtable->parent.update((Observer_t*)log_output, data);
    }
    while ( (log_output = log_output->listT.vtable->get_next(
                              &log_output->node)) != NULL );
}
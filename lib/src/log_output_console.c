#include "log_output_console.h"
#include "log_consumer.h"



// forward declaration
static void _Log_observer_dtor(Observer_t* self);
static bool _Log_output_console_update(Observer_t* self, void* data);
static bool _Log_output_console_print(Output_t* self, void* data);



static const Output_Vtable Log_output_console_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_console_update,
    .dtor          = Log_output_console_dtor,
    .print         = _Log_output_console_print
};



static void
_Log_observer_dtor(Observer_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Observer_t));
}



bool
Log_output_console_ctor(Log_output_t* self,
                        Log_format_t* log_format)
{
    CHECK_SELF(self);

    bool retval = false;

    if (log_format == NULL)
    {
        // Debug_printf
        return retval;
    }

    retval = ListT_ctor(&self->listT);

    self->node.prev = NULL;
    self->node.next = NULL;

    self->log_format = log_format;

    self->vtable = &Log_output_console_vtable;

    return retval;
}



void
Log_output_console_dtor(Output_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_output_t));
}



static bool
_Log_output_console_update(Observer_t* self, void* data)
{
    CHECK_SELF(self);

    if (data == NULL)
    {
        // Debug_printf
        return false;
    }

    Log_output_t* log_output = (Log_output_t*)self;
    log_output->vtable->print((Output_t*)log_output, data);

    return true;
}



static bool
_Log_output_console_print(Output_t* self, void* data)
{
    CHECK_SELF(self);

    Log_output_t* log_output = (Log_output_t*)self;

    // log format layer
    log_output->log_format->vtable->convert((Format_t*)log_output->log_format,
                                            (Log_info_t*) & (((Log_consumer_t*)data)->log_info));
    log_output->log_format->vtable->print((Format_t*)log_output->log_format);

    return true;
}

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerOutputFileSystem.h"
#include "OS_LoggerConsumer.h"
#include "OS_LoggerFile.h"
#include "seos_fs.h"
#include <string.h>
#include <stdio.h>



// forward declaration
static void _Log_observer_dtor(Observer_t* self);
static bool _Log_output_filesystem_update(Observer_t* self, void* data);
static bool _Log_output_filesystem_print(Output_t* self, void* data);



static const Output_Vtable Log_output_filesystem_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_filesystem_update,
    .dtor          = Log_output_filesystem_dtor,
    .print         = _Log_output_filesystem_print
};



static void
_Log_observer_dtor(Observer_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Observer_t));
}



bool
Log_output_filesystem_ctor(Log_output_t* self,
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

    self->vtable = &Log_output_filesystem_vtable;

    return retval;
}



void
Log_output_filesystem_dtor(Output_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Log_output_t));
}



static bool
_Log_output_filesystem_update(Observer_t* self, void* data)
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
_Log_output_filesystem_print(Output_t* self, void* data)
{
    CHECK_SELF(self);

    if (data == NULL)
    {
        // Debug_printf
        return false;
    }

    hFile_t fhandle;
    Log_output_t* log_output = (Log_output_t*)self;
    Log_consumer_t* log_consumer = (Log_consumer_t*)data;

    // check if log_file is installed
    if (log_consumer->log_file == NULL)
    {
        // Debug_printf
        return false;
    }

    // log format layer
    log_output->log_format->vtable->convert((Format_t*)log_output->log_format,
                                            &log_consumer->log_info);

    fhandle = file_open(((Log_file_t*)
                         log_consumer->log_file)->log_file_info.phandle,
                        ((Log_file_t*)log_consumer->log_file)->log_file_info.filename, FA_WRITE);
    if (!is_valid_file_handle(fhandle))
    {
        printf("Fail to open file: %s!\n",
               ((Log_file_t*)log_consumer->log_file)->log_file_info.filename);
        return false;
    }

    if (file_write(fhandle, (long)((Log_file_t*)
                                   log_consumer->log_file)->log_file_info.offset,
                   (long)strlen(log_output->log_format->buffer),
                   log_output->log_format->buffer) != SEOS_SUCCESS)
    {
        printf("Fail to write file: %s!\n",
               ((Log_file_t*)log_consumer->log_file)->log_file_info.filename);
        return false;
    }

    if (file_close(fhandle) != SEOS_SUCCESS)
    {
        printf("Fail to close file: %s!\n",
               ((Log_file_t*)log_consumer->log_file)->log_file_info.filename);
        return false;
    }

    ((Log_file_t*)log_consumer->log_file)->log_file_info.offset += strlen(
                log_output->log_format->buffer);

    return true;
}
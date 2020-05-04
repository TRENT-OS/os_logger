/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Client/OS_LoggerFileClient.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>



static OS_LoggerFileClient_vtable_t Log_file_client_vtable =
{
    .dtor          = OS_LoggerFileClient_dtor,
    .read_log_file = OS_LoggerFileClient_read
};



seos_err_t
OS_LoggerFileClient_ctor(
    OS_LoggerFileClient_Handle_t* self,
    void* src_buf,
    void* dest_buf,
    OS_LoggerFileClientCallback_Handle_t* log_file_client_callback)
{
    OS_Logger_CHECK_SELF(self);

    if (src_buf == NULL || dest_buf == NULL || log_file_client_callback == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    self->src_buf = src_buf;
    self->dest_buf = dest_buf;
    self->vtable = &Log_file_client_vtable;
    self->callback_vtable = log_file_client_callback;

    return SEOS_SUCCESS;
}



void
OS_LoggerFileClient_dtor(OS_LoggerFileClient_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerFileClient_Handle_t));
}



seos_err_t
OS_LoggerFileClient_read(OS_LoggerFileClient_Handle_t* self,
                         const char* filename,
                         uint64_t offset,
                         uint64_t len)
{
    OS_Logger_CHECK_SELF(self);

    if (filename == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (self->callback_vtable->read_log_file == NULL)
    {
        return SEOS_ERROR_INVALID_HANDLE;
    }

    int64_t log_file_size;
    int64_t read_bytes;

    do
    {
        read_bytes = self->callback_vtable->read_log_file(filename, offset, len,
                                                          &log_file_size);
        if (read_bytes <= 0)
        {
            break;
        }

        memcpy((char*)self->dest_buf + offset, self->src_buf, (size_t)read_bytes);

        offset += (uint64_t)read_bytes;

        if ((int64_t)offset >= log_file_size)
        {
            break;
        }
    }
    while (1);

    return SEOS_SUCCESS;
}

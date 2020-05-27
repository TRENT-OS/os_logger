/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerFile.h"
#include "Logger/Server/OS_LoggerConsumerChain.h"
#include "Logger/Server/OS_LoggerConsumer.h"
#include <string.h>
#include <stdio.h>



// forward declaration
static int64_t _Log_file_read_log_file(
    OS_LoggerFile_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t log_file_size);

static void    _init_partition(bool flag);
static bool    _is_init_partition(void);
static void*    _Log_file_get_consumer_by_filename(const char* filename);



static const OS_LoggerFile_vtable_t Log_file_vtable =
{
    .dtor                     = OS_LoggerFile_dtor,
    .create_log_file          = OS_LoggerFile_create,
    .read_log_file            = _Log_file_read_log_file,
    .get_consumer_by_filename = _Log_file_get_consumer_by_filename
};



static bool _init;



static void
_init_partition(bool flag)
{
    _init = flag;
}



static bool
_is_init_partition(void)
{
    return _init;
}



static void*
_Log_file_get_consumer_by_filename(const char* filename)
{
    if (filename == NULL)
    {
        // Debug_printf
        return NULL;
    }

    OS_LoggerConsumerChain_Handle_t* consumer_chain;
    OS_LoggerConsumer_Handle_t* log_consumer;

    consumer_chain = OS_LoggerConsumerChain_getInstance();

    if (consumer_chain == NULL)
    {
        return NULL;
    }

    if (consumer_chain->node.first == NULL)
    {
        return NULL;
    }

    log_consumer = consumer_chain->node.first;

    do
    {
        if (log_consumer->log_file == NULL)
        {
            continue;
        }

        if (0 == strcmp(
                ((OS_LoggerFile_Handle_t*)
                 log_consumer->log_file)->log_file_info.filename,
                filename))
        {
            return (void*)log_consumer;
            break;
        }
    }
    while ( (log_consumer = consumer_chain->listT.vtable->get_next(
                                &log_consumer->node)) != NULL );

    return NULL;
}



int64_t
API_LOG_SERVER_READ_LOG_FILE(
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t* log_file_size)
{
    if (filename == NULL)
    {
        return -1;
    }

    OS_LoggerConsumer_Handle_t* log_consumer =
        OS_LoggerConsumerChain_getSender();

    if (log_consumer == NULL)
    {
        return -1;
    }

    OS_LoggerConsumer_Handle_t* log_consumer_filename =
        (OS_LoggerConsumer_Handle_t*)_Log_file_get_consumer_by_filename(
            filename);

    if (log_consumer_filename == NULL)
    {
        return -1;
    }

    *log_file_size = OS_Filesystem_getSizeOfFile(
                         ((OS_LoggerFile_Handle_t*)
                          log_consumer_filename->log_file)->log_file_info.phandle,
                         filename);
    if (*log_file_size < 0)
    {
        return -1;
    }

    ((OS_LoggerFile_Handle_t*)log_consumer_filename->log_file)->log_file_info.lenght
        =
            (uint64_t) * log_file_size;

    if (offset > (uint64_t)*log_file_size)
    {
        return -1;
    }

    if (*log_file_size <= (int64_t)(offset + len))
    {
        len = (uint64_t)((uint64_t) * log_file_size - offset);
    }

    hFile_t fhandle;
    fhandle = OS_Filesystem_openFile(
                  ((OS_LoggerFile_Handle_t*)
                   log_consumer_filename->log_file)->log_file_info.phandle,
                  filename,
                  FA_READ);
    if (!OS_Filesystem_validateFileHandle(fhandle))
    {
        printf("Fail to open file: %s!\n", filename);
        return -1;
    }

    if (OS_Filesystem_readFile(
            fhandle,
            (long)offset,
            (long)len,
            log_consumer->buf) != OS_SUCCESS)
    {
        printf("Fail to read file: %s!\n", filename);
        return -1;
    }

    if (OS_Filesystem_closeFile(fhandle) != OS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", filename);
        return -1;
    }

    return (int64_t)len;
}



OS_Error_t
OS_LoggerFile_ctor(
    OS_LoggerFile_Handle_t* self,
    uint8_t drv_id,
    const char* filename)
{
    OS_Logger_CHECK_SELF(self);

    if (NULL == filename
        || (strlen(filename) >= sizeof(self->log_file_info.filename)))
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    self->vtable = &Log_file_vtable;

    self->log_file_info.drv_id = drv_id;
    strcpy(self->log_file_info.filename, filename);

    _init_partition(false);

    return OS_SUCCESS;
}



void
OS_LoggerFile_dtor(OS_LoggerFile_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    if (_is_init_partition())
    {
        if (OS_Filesystem_unmount(self->log_file_info.phandle) != OS_SUCCESS)
        {
            printf("Fail to unmount partition: %d!\n", self->log_file_info.drv_id);
            return;
        }

        if (OS_Filesystem_close(self->log_file_info.phandle) != OS_SUCCESS)
        {
            printf("Fail to close partition: %d!\n", self->log_file_info.drv_id);
            return;
        }

        _init_partition(false);
    }

    memset(self, 0, sizeof (OS_LoggerFile_Handle_t));
}



OS_Error_t
OS_LoggerFile_create(OS_LoggerFile_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    if (!_is_init_partition())
    {
        self->log_file_info.phandle = OS_Filesystem_open(self->log_file_info.drv_id);
        if (!OS_Filesystem_validatePartitionHandle(self->log_file_info.phandle))
        {
            return OS_ERROR_INVALID_HANDLE;
        }

        const OS_Error_t result =
            OS_Filesystem_mount(self->log_file_info.phandle);

        if (OS_SUCCESS != result)
        {
            return result;
        }

        _init_partition(true);
    }

    // create empty file
    hFile_t fhandle = OS_Filesystem_openFile(
                          self->log_file_info.phandle,
                          self->log_file_info.filename,
                          FA_CREATE_ALWAYS);

    if (!OS_Filesystem_validateFileHandle(fhandle))
    {
        printf("Fail to open file: %s!", self->log_file_info.filename);
        return OS_ERROR_INVALID_HANDLE;
    }

    const OS_Error_t result = OS_Filesystem_closeFile(fhandle);
    if (OS_SUCCESS != result)
    {
        printf("Fail to close file: %s!\n", self->log_file_info.filename);
        return result;
    }

    self->log_file_info.offset = 0;

    return OS_SUCCESS;
}



static int64_t
_Log_file_read_log_file(
    OS_LoggerFile_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t log_file_size)
{
    OS_Logger_CHECK_SELF(self);

    return API_LOG_SERVER_READ_LOG_FILE(filename, offset, len, &log_file_size);
}

#include "log_file.h"
#include "consumer_chain.h"
#include "log_consumer.h"
#include <string.h>
#include <stdio.h>



// forward declaration
static int64_t _Log_file_read_log_file(Log_file_t* self, const char* filename,
                                       uint64_t offset, uint64_t len, int64_t log_file_size);
static void    _init_partition(bool flag);
static bool    _is_init_partition(void);
static void*    _Log_file_get_consumer_by_filename(const char* filename);



static const Log_file_Vtable Log_file_vtable =
{
    .dtor                     = Log_file_dtor,
    .create_log_file          = Log_file_create_log_file,
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

    Consumer_chain_t* consumer_chain;
    Log_consumer_t* log_consumer;

    consumer_chain = get_instance_Consumer_chain();

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

        if (strcmp(((Log_file_t*)log_consumer->log_file)->log_file_info.filename,
                   filename) == 0)
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
API_LOG_SERVER_READ_LOG_FILE(const char* filename,
                             uint64_t offset,
                             uint64_t len,
                             int64_t* log_file_size)
{
    if (filename == NULL)
    {
        return -1;
    }

    Log_consumer_t* log_consumer = Consumer_chain_get_sender();
    if (log_consumer == NULL)
    {
        return -1;
    }

    Log_consumer_t* log_consumer_filename = (Log_consumer_t*)
                                            _Log_file_get_consumer_by_filename(filename);
    if (log_consumer_filename == NULL)
    {
        return -1;
    }

    *log_file_size = file_getSize(((Log_file_t*)
                                   log_consumer_filename->log_file)->log_file_info.phandle, filename);
    if (*log_file_size < 0)
    {
        return -1;
    }

    ((Log_file_t*)log_consumer_filename->log_file)->log_file_info.lenght =
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
    fhandle = file_open(((Log_file_t*)
                         log_consumer_filename->log_file)->log_file_info.phandle, filename, FA_READ);
    if (!is_valid_file_handle(fhandle))
    {
        printf("Fail to open file: %s!\n", filename);
        return -1;
    }

    if (file_read(fhandle, (long)offset, (long)len,
                  log_consumer->buf) != SEOS_FS_SUCCESS)
    {
        printf("Fail to read file: %s!\n", filename);
        return -1;
    }

    if (file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", filename);
        return -1;
    }

    return (int64_t)len;
}



bool
Log_file_ctor(Log_file_t* self,
              uint8_t drv_id,
              const char* filename)
{
    CHECK_SELF(self);

    if (filename == NULL)
    {
        // Debug_printf
        return false;
    }

    self->vtable = &Log_file_vtable;

    self->log_file_info.drv_id = drv_id;
    strcpy(self->log_file_info.filename, filename);

    _init_partition(false);

    return true;
}



void
Log_file_dtor(Log_file_t* self)
{
    CHECK_SELF(self);

    if (_is_init_partition())
    {
        if (partition_fs_unmount(self->log_file_info.phandle) != SEOS_FS_SUCCESS)
        {
            printf("Fail to unmount partition: %d!\n", self->log_file_info.drv_id);
            return;
        }

        if (partition_close(self->log_file_info.phandle) != SEOS_FS_SUCCESS)
        {
            printf("Fail to close partition: %d!\n", self->log_file_info.drv_id);
            return;
        }

        _init_partition(false);
    }

    memset(self, 0, sizeof (Log_file_t));
}



bool
Log_file_create_log_file(Log_file_t* self)
{
    CHECK_SELF(self);

    if (!_is_init_partition())
    {
        self->log_file_info.phandle = partition_open(self->log_file_info.drv_id);
        if (!is_valid_partition_handle(self->log_file_info.phandle))
        {
            return -1;
        }

        if (partition_fs_mount(self->log_file_info.phandle) != SEOS_FS_SUCCESS)
        {
            return false;
        }

        _init_partition(true);
    }

    // create empty file
    hFile_t fhandle;
    fhandle = file_open(self->log_file_info.phandle, self->log_file_info.filename,
                        FA_CREATE_ALWAYS);
    if (!is_valid_file_handle(fhandle))
    {
        printf("Fail to open file: %s!\n", self->log_file_info.filename);
        return false;
    }

    if (file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", self->log_file_info.filename);
        return false;
    }

    self->log_file_info.offset = 0;

    return true;
}



static int64_t
_Log_file_read_log_file(Log_file_t* self,
                        const char* filename,
                        uint64_t offset,
                        uint64_t len,
                        int64_t log_file_size)
{
    CHECK_SELF(self);

    return API_LOG_SERVER_READ_LOG_FILE(filename, offset, len, &log_file_size);
}

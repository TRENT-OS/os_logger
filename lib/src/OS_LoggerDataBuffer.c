/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Common/OS_LoggerDataBuffer.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static
void
_setLogLevel(void* buf, uint8_t log_level, size_t offset)
{
    assert(buf);

    sprintf(
        (char*)buf + offset,
        "%*u",
        OS_Logger_LOG_LEVEL_LENGTH,
        log_level);
}

/**********/
/* Setter */
/**********/
void
OS_LoggerDataBuffer_setClientLogLevel(void* buf, uint8_t log_level_client)
{
    _setLogLevel(buf, log_level_client, OS_Logger_LOG_LEVEL_CLIENT_OFFSET);
}

void
OS_LoggerDataBuffer_setServerLogLevel(void* buf, uint8_t log_level_server)
{
    _setLogLevel(buf, log_level_server, OS_Logger_LOG_LEVEL_SERVER_OFFSET);
}

OS_Error_t
OS_LoggerDataBuffer_setLogMessage(void* buf, const char* msg)
{
    assert(buf);
    assert(msg);

    const int retval = snprintf(
                           (char*)buf + OS_Logger_MESSAGE_OFFSET,
                           OS_Logger_MESSAGE_LENGTH,
                           "%s",
                           msg);

    if (retval < 0)
    {
        return OS_ERROR_GENERIC;
    }

    if (retval > OS_Logger_MESSAGE_LENGTH)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_SUCCESS;
}

/**********/
/* Getter */
/**********/
OS_Error_t
OS_LoggerDataBuffer_getServerLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    char tmp_log_level_srv[OS_Logger_LOG_LEVEL_LENGTH + 1] = "0"; // + '\0'

    memcpy(tmp_log_level_srv, (char*)buf + OS_Logger_LOG_LEVEL_SERVER_OFFSET,
           OS_Logger_LOG_LEVEL_LENGTH);

    log_databuffer->log_level_srv = (uint8_t)atoi(tmp_log_level_srv);

    return OS_SUCCESS;
}



OS_Error_t
OS_LoggerDataBuffer_getClientLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    char tmp_log_level_client[OS_Logger_LOG_LEVEL_LENGTH] = "0"; // + '\0'

    memcpy(tmp_log_level_client, (char*)buf + OS_Logger_LOG_LEVEL_CLIENT_OFFSET,
           OS_Logger_LOG_LEVEL_LENGTH);

    log_databuffer->log_level_client = (uint8_t)atoi(tmp_log_level_client);

    return OS_SUCCESS;
}



OS_Error_t
OS_LoggerDataBuffer_getLogMessage(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    memcpy(log_databuffer->log_message, (char*)buf + OS_Logger_MESSAGE_OFFSET,
           OS_Logger_MESSAGE_LENGTH);

    return OS_SUCCESS;
}



OS_Error_t
OS_LoggerDataBuffer_getInfo(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    OS_Error_t retval = OS_LoggerDataBuffer_getServerLogLevel(
                            buf,
                            log_databuffer);

    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    retval = OS_LoggerDataBuffer_getClientLogLevel(buf, log_databuffer);
    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    retval = OS_LoggerDataBuffer_getLogMessage(buf, log_databuffer);
    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    return OS_SUCCESS;
}



OS_Error_t
OS_LoggerDataBuffer_clear(void* buf)
{
    if (buf == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    memset((char*)buf + OS_Logger_LOG_LEVEL_CLIENT_OFFSET, 0,
           DATABUFFER_SIZE - OS_Logger_LOG_LEVEL_LENGTH);

    return OS_SUCCESS;
}

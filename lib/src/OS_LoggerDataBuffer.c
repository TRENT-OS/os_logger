/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Common/OS_LoggerDataBuffer.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



/**********/
/* Setter */
/**********/
int
OS_LoggerDataBuffer_setClientLogLevel(void* buf, uint8_t log_level_client)
{
    int retval = -1;

    if (buf == NULL)
    {
        return retval;
    }

    retval = sprintf(
                 (char*)buf + OS_Logger_LOG_LEVEL_CLIENT_OFFSET,
                 "%*u",
                 OS_Logger_LOG_LEVEL_LENGTH,
                 log_level_client);

    if (retval < 0 || retval > OS_Logger_LOG_LEVEL_LENGTH)
    {
        return -1;
    }

    return retval;
}



int
OS_LoggerDataBuffer_setServerLogLevel(void* buf, uint8_t log_level_server)
{
    int retval = -1;

    if (buf == NULL)
    {
        return retval;
    }

    retval = sprintf(
                 (char*)buf + OS_Logger_LOG_LEVEL_SERVER_OFFSET,
                 "%*u",
                 OS_Logger_LOG_LEVEL_LENGTH,
                 log_level_server);

    if (retval < 0 || retval > OS_Logger_LOG_LEVEL_LENGTH)
    {
        return -1;
    }

    return retval;
}



int
OS_LoggerDataBuffer_setLogMessage(void* buf, const char* msg)
{
    int retval = -1;

    if (buf == NULL || msg == NULL)
    {
        return retval;
    }

    retval = snprintf(
                 (char*)buf + OS_Logger_MESSAGE_OFFSET,
                 OS_Logger_MESSAGE_LENGTH,
                 "%s",
                 msg);

    if (retval < 0 || retval > OS_Logger_MESSAGE_LENGTH)
    {
        return -2;
    }

    return retval;
}



/**********/
/* Getter */
/**********/
seos_err_t
OS_LoggerDataBuffer_getServerLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    char tmp_log_level_srv[OS_Logger_LOG_LEVEL_LENGTH + 1] = "0"; // + '\0'

    memcpy(tmp_log_level_srv, (char*)buf + OS_Logger_LOG_LEVEL_SERVER_OFFSET,
           OS_Logger_LOG_LEVEL_LENGTH);

    log_databuffer->log_level_srv = (uint8_t)atoi(tmp_log_level_srv);

    return SEOS_SUCCESS;
}



seos_err_t
OS_LoggerDataBuffer_getClientLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    char tmp_log_level_client[OS_Logger_LOG_LEVEL_LENGTH] = "0"; // + '\0'

    memcpy(tmp_log_level_client, (char*)buf + OS_Logger_LOG_LEVEL_CLIENT_OFFSET,
           OS_Logger_LOG_LEVEL_LENGTH);

    log_databuffer->log_level_client = (uint8_t)atoi(tmp_log_level_client);

    return SEOS_SUCCESS;
}



seos_err_t
OS_LoggerDataBuffer_getLogMessage(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    memcpy(log_databuffer->log_message, (char*)buf + OS_Logger_MESSAGE_OFFSET,
           OS_Logger_MESSAGE_LENGTH);

    return SEOS_SUCCESS;
}



seos_err_t
OS_LoggerDataBuffer_getInfo(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    seos_err_t retval = OS_LoggerDataBuffer_getServerLogLevel(
                            buf,
                            log_databuffer);

    if (retval != SEOS_SUCCESS)
    {
        return retval;
    }

    retval = OS_LoggerDataBuffer_getClientLogLevel(buf, log_databuffer);
    if (retval != SEOS_SUCCESS)
    {
        return retval;
    }

    retval = OS_LoggerDataBuffer_getLogMessage(buf, log_databuffer);
    if (retval != SEOS_SUCCESS)
    {
        return retval;
    }

    return SEOS_SUCCESS;
}



seos_err_t
OS_LoggerDataBuffer_clear(void* buf)
{
    if (buf == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    memset((char*)buf + OS_Logger_LOG_LEVEL_CLIENT_OFFSET, 0,
           DATABUFFER_SIZE - OS_Logger_LOG_LEVEL_LENGTH);

    return SEOS_SUCCESS;
}

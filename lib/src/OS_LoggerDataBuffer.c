/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerDataBuffer.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



/**********/
/* Setter */
/**********/
int
Log_databuffer_set_log_level_client(void* buf, uint8_t log_level_client)
{
    int retval = -1;

    if (buf == NULL)
    {
        return retval;
    }

    retval = sprintf(
                 (char*)buf + LOG_LEVEL_CLIENT_OFFSET,
                 "%*u",
                 LOG_LEVEL_CLIENT_LENGTH,
                 log_level_client);

    if (retval < 0 || retval > LOG_LEVEL_CLIENT_LENGTH)
    {
        return -1;
    }

    return retval;
}



int
Log_databuffer_set_log_level_server(void* buf, uint8_t log_level_server)
{
    int retval = -1;

    if (buf == NULL)
    {
        return retval;
    }

    retval = sprintf(
                 (char*)buf + LOG_LEVEL_SERVER_OFFSET,
                 "%*u",
                 LOG_LEVEL_SERVER_LENGTH,
                 log_level_server);

    if (retval < 0 || retval > LOG_LEVEL_SERVER_LENGTH)
    {
        return -1;
    }

    return retval;
}



int
Log_databuffer_set_log_message(void* buf, const char* msg)
{
    int retval = -1;

    if (buf == NULL || msg == NULL)
    {
        return retval;
    }

    retval = snprintf(
                 (char*)buf + LOG_LEVEL_MESSAGE_OFFSET,
                 LOG_MESSAGE_LENGTH,
                 "%s",
                 msg);

    if (retval < 0 || retval > LOG_MESSAGE_LENGTH)
    {
        return -2;
    }

    return retval;
}



/**********/
/* Getter */
/**********/
bool
Log_databuffer_get_log_level_server(void* buf, Log_databuffer_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return false;
    }

    char tmp_log_level_srv[LOG_LEVEL_SERVER_LENGTH + 1] = "0"; // + '\0'

    memcpy(tmp_log_level_srv, (char*)buf + LOG_LEVEL_SERVER_OFFSET,
           LOG_LEVEL_SERVER_LENGTH);

    log_databuffer->log_level_srv = (uint8_t)atoi(tmp_log_level_srv);

    return true;
}



bool
Log_databuffer_get_log_level_client(void* buf, Log_databuffer_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return false;
    }

    char tmp_log_level_client[LOG_LEVEL_CLIENT_LENGTH] = "0"; // + '\0'

    memcpy(tmp_log_level_client, (char*)buf + LOG_LEVEL_CLIENT_OFFSET,
           LOG_LEVEL_CLIENT_LENGTH);

    log_databuffer->log_level_client = (uint8_t)atoi(tmp_log_level_client);

    return true;
}



bool
Log_databuffer_get_log_message(void* buf, Log_databuffer_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return false;
    }

    memcpy(log_databuffer->log_message, (char*)buf + LOG_LEVEL_MESSAGE_OFFSET,
           LOG_MESSAGE_LENGTH);

    return true;
}



bool
Log_databuffer_get_info(void* buf, Log_databuffer_t* log_databuffer)
{
    if (buf == NULL || log_databuffer == NULL)
    {
        return false;
    }

    bool retval = false;

    retval = Log_databuffer_get_log_level_server(buf, log_databuffer);
    if (retval == false)
    {
        return false;
    }

    retval = Log_databuffer_get_log_level_client(buf, log_databuffer);
    if (retval == false)
    {
        return false;
    }

    retval = Log_databuffer_get_log_message(buf, log_databuffer);
    if (retval == false)
    {
        return false;
    }

    return true;
}



bool
Log_databuffer_clear_databuffer(void* buf)
{
    if (buf == NULL)
    {
        return false;
    }

    memset((char*)buf + LOG_LEVEL_CLIENT_OFFSET, 0,
           DATABUFFER_SIZE - LOG_LEVEL_SERVER_LENGTH);

    return true;
}
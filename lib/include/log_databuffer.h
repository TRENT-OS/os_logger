/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Data buffer implementation.
 */
#pragma once

/**
 * @defgroup    log_databuffer Data buffer implementation
 *
 * @brief       This layer provides functions to manage data buffer.
 * @details     There are functions for getting and setting information from an
 *              object to data buffer and vice versa.
 *
 * @ingroup     seos_logger_server
*/
#include "log_symbol.h"
#include "timestamp.h"
#include <stdint.h>


/**
 * @details Log_databuffer_t contain informations from a data buffer in a
 *          structured way.
 *
 * @ingroup log_databuffer
*/
typedef struct
{
    uint8_t log_level_srv;                   /**< server log level */
    uint8_t log_level_client;                /**< client log level */
    char    log_message[LOG_MESSAGE_LENGTH]; /**< buffer for log message */
} Log_databuffer_t;


/**
 * @details Log_info_t contain all informations about a log.
 *
 * @ingroup log_databuffer
*/
typedef struct
{
    char             log_id_and_name[LOG_ID_AND_NAME_LENGTH];
    Timestamp_t      timestamp;
    Log_databuffer_t log_databuffer;
} Log_info_t;


/**
 * @details %Log_databuffer_set_log_level_server copy the server log level to
 *          the data buffer.
 *
 * @param   buf:                data buffer
 * @param   log_level_server:   server log level
 *
 * @return  an status code
 *
 * @retval  >= 0, if all allright
 * @retval    -1, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
int
Log_databuffer_set_log_level_server(void* buf, uint8_t log_level_server);


/**
 * @details %Log_databuffer_set_log_level_client copy the client log level to
 *          the data buffer.
 *
 * @param   buf:                data buffer
 * @param   log_level_client:   client log level
 *
 * @return  an status code
 *
 * @retval  >= 0, if all allright
 * @retval    -1, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
int
Log_databuffer_set_log_level_client(void* buf, uint8_t log_level_client);


/**
 * @details %Log_databuffer_set_log_message copy the log message to the data
 *          buffer.
 *
 * @param   buf:    data buffer
 * @param   msg:    log message
 *
 * @return  an status code
 *
 * @retval  >= 0, if all allright
 * @retval    -1, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
int
Log_databuffer_set_log_message(void* buf, const char* msg);


/**
 * @details %Log_databuffer_get_log_level_server copy the server log level from
 *          the data buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
bool
Log_databuffer_get_log_level_server(void* buf,
                                    Log_databuffer_t* log_databuffer);


/**
 * @details %Log_databuffer_get_log_level_client copy the client log level from
 *          the data buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
bool
Log_databuffer_get_log_level_client(void* buf,
                                    Log_databuffer_t* log_databuffer);


/**
 * @details %Log_databuffer_get_log_message copy the log message from the data
 *          buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup log_databuffer
*/
bool
Log_databuffer_get_log_message(void* buf, Log_databuffer_t* log_databuffer);


/**
 * @details %Log_databuffer_get_info is an helper function, which getting all
 *          informatations from the data buffer by calling:
 *
 *          \code
 *              Log_databuffer_get_log_level_server
 *              Log_databuffer_get_log_level_client
 *              Log_databuffer_get_log_message
 *          \endcode
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has occurred
 *
 * @ingroup log_databuffer
*/
bool
Log_databuffer_get_info(void* buf, Log_databuffer_t* log_databuffer);


/**
 * @details %Log_databuffer_clear_databuffer clean the data buffer.
 *
 * @param   buf:            data buffer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has occurred
 *
 * @ingroup log_databuffer
*/
bool
Log_databuffer_clear_databuffer(void* buf);

/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Defines of log server internals
 */
#pragma once

#define STR_(x)                         #x
#define STR(x)                          STR_(x)

/**
 * @defgroup    OS_LoggerSymbols Defines of log server internals
 *
 * @brief       Defines the configuration of the log server internals.
 * @details     This file contain configuration for the log databuffer and for
 *              the log format example implementation.
 *
 * @ingroup     OS_LoggerServer, OS_LoggerClient
 */

// include parameter for configuration file
#if !defined(LOGGER_CONFIG_H_FILE)
#error "a configuration file must be provided! See seos_log_server_config.h.example"
#else
#include STR(LOGGER_CONFIG_H_FILE)
#endif


// configuration of log server
#if !defined (DATABUFFER_SIZE)
#define DATABUFFER_SIZE             4096
#endif

#define LOG_LEVEL_SERVER_OFFSET         0
#define LOG_LEVEL_CLIENT_OFFSET         LOG_LEVEL_SERVER_LENGTH
#define LOG_LEVEL_MESSAGE_OFFSET        LOG_LEVEL_CLIENT_OFFSET + LOG_LEVEL_CLIENT_LENGTH

#define LOG_LEVEL_SERVER_LENGTH         3 // Without the null terminator!
#define LOG_LEVEL_CLIENT_LENGTH         3 // Without the null terminator!
#define LOG_MESSAGE_LENGTH              (DATABUFFER_SIZE - (LOG_LEVEL_SERVER_LENGTH + LOG_LEVEL_CLIENT_LENGTH))

#define LOG_ID_AND_NAME_LENGTH          20
#define LOG_ID_LENGTH                   6
#define LOG_NAME_LENGTH                 (LOG_ID_AND_NAME_LENGTH - LOG_ID_LENGTH)

// includes
#include <assert.h>

#define CHECK_SELF(self)                assert(self)

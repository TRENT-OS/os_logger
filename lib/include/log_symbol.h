#pragma once

#define STR_(x)                         #x
#define STR(x)                          STR_(x)

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
#define LOG_LEVEL_CLIENT_OFFSET         1
#define LOG_LEVEL_MESSAGE_OFFSET        2

#define LOG_LEVEL_SERVER_LENGTH         2
#define LOG_LEVEL_CLIENT_LENGTH         2
#define LOG_MESSAGE_LENGTH              (DATABUFFER_SIZE - (LOG_LEVEL_SERVER_LENGTH + LOG_LEVEL_CLIENT_LENGTH))

#define LOG_ID_NAME_LENGTH              20


// configure log format string
#define FORMAT_ID_LENGTH                LOG_ID_NAME_LENGTH
#define FORMAT_TIMESTAMP_LENTGH         20
#define FORMAT_LOG_LEVEL_SERVER_LENGTH  2
#define FORMAT_LOG_LEVEL_CLIENT_LENGTH  2
#define FORMAT_LOG_MESSAGE_LENGTH       LOG_MESSAGE_LENGTH
#define FORMAT_BUFFER_SIZE              (FORMAT_ID_LENGTH +                 \
                                         FORMAT_TIMESTAMP_LENTGH +          \
                                         FORMAT_LOG_LEVEL_SERVER_LENGTH +   \
                                         FORMAT_LOG_LEVEL_CLIENT_LENGTH +   \
                                         FORMAT_LOG_MESSAGE_LENGTH)


// includes
#include <assert.h>

#define CHECK_SELF(self)                assert(self)

#
# OS Logger
#
# Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
# 
# SPDX-License-Identifier: GPL-2.0-or-later
#
# For commercial licensing, contact: info.cyber@hensoldt.net
#

cmake_minimum_required(VERSION 3.13.0)

# enable new behavior introduced in CMake 3.13, where relative paths will be
# converted into absolute paths in target_sources(INTERFACE ....)
cmake_policy(SET CMP0076 NEW)


#-------------------------------------------------------------------------------
# client lib
project(os_logger C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/OS_LoggerEmitter.c
        lib/src/OS_LoggerFileClient.c
        lib/src/OS_LoggerFileClientCallback.c
        lib/src/OS_LoggerFilter.c
        lib/src/OS_LoggerTimestamp
)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE
        # affects these header files:
        #    os_core_api/include/Logger/Client/OS_Logger.h
        #    libs/lib_debug/include/lib_debug/Debug.h
        Debug_Config_PRINT_TO_LOG_SERVER
        Debug_Config_LOG_SERVER_H="Logger/Client/OS_Logger.h"
)

if (OS_Logger_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "OS_Logger_CONFIG_H_FILE=${OS_Logger_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        os_core_api
)


#------------------------------------------------------------------------------
# server lib to log to console
project(os_log_server_backend_console C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/OS_LoggerConsumerChain.c
        lib/src/OS_LoggerListT.c
        lib/src/OS_LoggerConsumer.c
        lib/src/OS_LoggerConsumerCallback.c
        lib/src/OS_LoggerEmitter.c
        lib/src/OS_LoggerFilter.c
        lib/src/OS_LoggerFormat.c
        lib/src/OS_LoggerSubject.c
        lib/src/OS_LoggerTimestamp
        lib/src/OS_LoggerOutput.c
        lib/src/OS_LoggerOutputConsole.c
)

if (OS_Logger_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "OS_Logger_CONFIG_H_FILE=${OS_Logger_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        os_core_api
)


#------------------------------------------------------------------------------
# server lib to log to filesystem
project(os_log_server_backend_filesystem C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/OS_LoggerFile.c
        lib/src/OS_LoggerOutputFileSystem.c
)

if (OS_Logger_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "OS_Logger_CONFIG_H_FILE=${OS_Logger_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        os_core_api
        os_filesystem
        os_log_server_backend_console
)

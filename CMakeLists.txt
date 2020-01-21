cmake_minimum_required(VERSION 3.13.0)

# enable new behavior introduced in CMake 3.13, where relative paths will be
# converted into absolute paths in target_sources(INTERFACE ....)
cmake_policy(SET CMP0076 NEW)


# client lib
project(seos_logger C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/log_databuffer.c
        lib/src/log_emitter.c
        lib/src/log_emitter_callback.c
        lib/src/log_file_client.c
        lib/src/log_file_client_callback.c
        lib/src/log_filter.c
        lib/src/timestamp.c
)

if (LOGGER_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "LOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        seos_core_api
)


#------------------------------------------------------------------------------


# server lib to log to console
project(seos_log_server_backend_console C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/consumer_chain.c
        lib/src/listT.c
        lib/src/log_consumer.c
        lib/src/log_consumer_callback.c
        lib/src/log_databuffer.c
        lib/src/log_emitter.c
        lib/src/log_emitter_callback.c
        lib/src/log_filter.c
        lib/src/log_format.c
        lib/src/log_subject.c
        lib/src/timestamp.c
        lib/src/log_output_console.c
)

if (LOGGER_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "LOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        seos_core_api
)


#------------------------------------------------------------------------------


# server lib to log to filesystem
project(seos_log_server_backend_filesystem C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
        lib/include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        lib/src/log_file.c
        lib/src/log_output_filesystem.c
)

if (LOGGER_CONFIG_H_FILE)
    target_compile_definitions(${PROJECT_NAME}
        INTERFACE
            "LOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}"
    )
endif()

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        seos_core_api
        seos_log_server_backend_console
        seos_filesystem_core_api
)


#------------------------------------------------------------------------------


# if invoked from the SOES sandbox, provide a documentation build target
if (COMMAND seos_create_doxygen_target)

    # there are no pre or post steps
    seos_create_doxygen_target(seos_logger_doc ":" ":")

endif()

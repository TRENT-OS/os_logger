/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server interface with console backend
 */
#pragma once

/**
 * @defgroup    seos_logger Logger interface
 *
 * @brief       Defined logger interface.
 * @details     Defined logger interface.
 */

/**
 * @defgroup   seos_logger_server Log server interface
 *
 * @brief      Defined log server interface.
 * @details    This level is required to make functions and objects available
 *             by server site.
 *
 *             There are available two different backend systems.
 *
 *             \code
 *                 console output
 *                 filesystem
 *             \endcode
 *
 *             If log server with console backend is used, than the header file
 *             "seos_log_server_backend_console.h" has to be included.
 *
 *             If log server with console backend is used, than the header file
 *             "seos_log_server_backend_filesystem.h" has to be included.
 *             \n
 *             By using filesystem backend, there is a strong dependency to file
 *             system api. The console backend is additional available.
 *
 * @ingroup    seos_logger
 */
#include "consumer_chain.h"
#include "log_symbol.h"
#include "log_filter.h"
#include "log_consumer.h"
#include "log_emitter.h"
#include "log_emitter_callback.h"
#include "log_format.h"
#include "log_subject.h"
#include "log_output_console.h"

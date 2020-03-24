/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server interface with console backend
 */
#pragma once

/**
 * @defgroup    OS_Logger Logger interface
 *
 * @brief       Defined logger interface.
 * @details     Defined logger interface.
 */

/**
 * @defgroup   OS_LoggerServer Log server interface
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
 *             "OS_LoggerServerBackendConsole.h" has to be included.
 *
 *             If log server with console backend is used, than the header file
 *             "OS_LoggerServerBackendFilesystem.h" has to be included.
 *             \n
 *             By using filesystem backend, there is a strong dependency to file
 *             system api. The console backend is additional available.
 *
 * @ingroup    OS_Logger
 */
#include "OS_LoggerConsumerChain.h"
#include "OS_LoggerSymbols.h"
#include "OS_LoggerFilter.h"
#include "OS_LoggerConsumer.h"
#include "OS_LoggerEmitter.h"
#include "OS_LoggerEmitterCallback.h"
#include "OS_LoggerFormat.h"
#include "OS_LoggerSubject.h"
#include "OS_LoggerOutputConsole.h"

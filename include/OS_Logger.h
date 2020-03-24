/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log client interface
 */
#pragma once

/**
 * @defgroup   OS_LoggerClient Log client interface
 *
 * @brief      Defined log client interface.
 * @details    This layer is required to make functions and objects available
 *             by client site.
 *
 * @ingroup    OS_Logger
*/
#include "OS_LoggerEmitter.h"
#include "OS_LoggerEmitterCallback.h"
#include "OS_LoggerFileClient.h"
#include "OS_LoggerFileClientCallback.h"
#include "OS_LoggerFilter.h"
#include "OS_LoggerSymbols.h"

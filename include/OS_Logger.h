/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log client interface
 */
#pragma once

/**
 * @defgroup   seos_logger_client Log client interface
 *
 * @brief      Defined log client interface.
 * @details    This layer is required to make functions and objects available
 *             by client site.
 *
 * @ingroup    seos_logger
*/
#include "OS_LoggerEmitter.h"
#include "OS_LoggerEmitterCallback.h"
#include "OS_LoggerFileClient.h"
#include "OS_LoggerFileClientCallback.h"
#include "OS_LoggerFilter.h"
#include "OS_LoggerSymbols.h"

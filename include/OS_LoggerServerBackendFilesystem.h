/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server interface with filesystem backend
 */
#pragma once

/**
 * @defgroup    OS_LoggerFileSystemBackend Log server interface with filesystem
 *              backend
 *
 * @brief      Defined log server interface with filesystem backend.
 *
 * @details    This level is required to make functions and objects available by
 *             server site.
 *
 *             The backend is filesystem. There is a strong dependency to file
 *             system api.
 *             \n
 *             \n
 *             The console backend is additional available.
 *
 * @ingroup     OS_LoggerServer
 */
#include "OS_LoggerServerBackendConsole.h"
#include "OS_LoggerOutputFileSystem.h"
#include "OS_LoggerFile.h"

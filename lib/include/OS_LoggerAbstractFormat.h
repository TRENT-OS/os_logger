/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract log format interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractFormat Abstract log format interface
 *
 * @brief       Defined abstract format interface for log format.
 * @details     This level provides an abstract interface for converting and
 *              printing log format.
 *
 *              This interface is abstract, so an implementation is necessary
 *              in a derived layer.
 *              \n
 *              Print function is a helper function to print the temporary log
 *              format.
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_LoggerDataBuffer.h"
#include "OS_LoggerSymbols.h"
#include <stdbool.h>
#include <string.h>


/**
 * @details OS_LoggerAbstractFormat_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerAbstractFormat
*/
typedef struct OS_LoggerAbstractFormat_Handle OS_LoggerAbstractFormat_Handle_t;


/**
 * @details OS_LoggerAbstractFormat_dtor defines the interface for function
 *          pointer to the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef void
(*OS_LoggerAbstractFormat_dtor)(OS_LoggerAbstractFormat_Handle_t* self);


/**
 * @details OS_LoggerAbstractFormat_convert defines the interface for functions
 *          pointer to the convert function.
 *
 *          The data of log level id, log level string and the log message are
 *          converted into a defined format.
 *
 * @param   self:       pointer to the class
 * @param   log_info:   pointer to the class OS_LoggerDataBuffer_info
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef bool
(*OS_LoggerAbstractFormat_convert)(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerDataBuffer_info* log_info);


/**
 * @details OS_LoggerAbstractFormat_print defines the interface for functions
 *          pointer to the print function.
 *
 *          Serves as helper function to print the temporary log format.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef void
(*OS_LoggerAbstractFormat_print)(OS_LoggerAbstractFormat_Handle_t* self);


/**
 * @details OS_LoggerAbstractFormat_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef struct
{
    OS_LoggerAbstractFormat_dtor    dtor;
    OS_LoggerAbstractFormat_convert convert;
    OS_LoggerAbstractFormat_print   print;
} OS_LoggerAbstractFormat_vtable_t;


/**
 * @details OS_LoggerAbstractFormat_Handle_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
struct OS_LoggerAbstractFormat_Handle
{
    const OS_LoggerAbstractFormat_vtable_t* vtable; /**< vtable */
};


/**
 * @details %FormatT_dtor is an abstract function for the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
inline void
FormatT_dtor(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractFormat_Handle_t));
}


/**
 * @details %FormatT_convert is an abstract function for the convert function.
 *
 * @param   self:       pointer to the class
 * @param   log_info:   pointer to the class OS_LoggerDataBuffer_info
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the convert function
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
inline
bool
FormatT_convert(
    OS_LoggerAbstractFormat_Handle_t* self,
    OS_LoggerDataBuffer_info* log_info)
{
    OS_Logger_CHECK_SELF(self);

    if (log_info == NULL)
    {
        // Debug_printf
        return false;
    }

    return self->vtable->convert(self, log_info);
}


/**
 * @details %FormatT_print is an abstract function for the print function.
 *
 * @param   self:       pointer to the class
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
inline void
FormatT_print(OS_LoggerAbstractFormat_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable->print(self);
}

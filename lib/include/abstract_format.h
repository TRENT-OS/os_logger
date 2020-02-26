/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract log format interface
 */
#pragma once

/**
 * @defgroup    abstract_format Abstract log format interface
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
 * @ingroup     seos_logger_server
*/
#include "log_databuffer.h"
#include "log_symbol.h"
#include <stdbool.h>
#include <string.h>


/**
 * @details Format_t defines the class datatype.
 *
 * @ingroup abstract_format
*/
typedef struct Format_t Format_t;


/**
 * @details FormatT_dtorT defines the interface for function pointer t
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup abstract_format
*/
typedef void
(*FormatT_dtorT)(Format_t* self);


/**
 * @details FormatT_convertT defines the interface for functions pointer to
 *          convert function.
 *
 *          The data of log level id, log level string and the log message are
 *          converted into a defined format.
 *
 * @param   self:       pointer to the class
 * @param   log_info:   pointer to the class Log_info_t
 *
 * @return  an status code
 *
 * @ingroup abstract_format
*/
typedef bool
(*FormatT_convertT)(Format_t* self, Log_info_t* log_info);


/**
 * @details FormatT_printT defines the interface for functions pointer to print
 *          function.
 *
 *          Serves as helper function to print the temporary log format.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup abstract_format
*/
typedef void
(*FormatT_printT)(Format_t* self);


/**
 * @details FormatT_Vtable contain the member functions to his class.
 *
 * @ingroup abstract_format
*/
typedef struct
{
    FormatT_dtorT    dtor;    /**< function pointer to desctructor */
    FormatT_convertT convert; /**< function pointer to convert function */
    FormatT_printT   print;   /**< function pointer to print function */
} FormatT_Vtable;


/**
 * @details Format_t contain the vtable to his class.
 *
 * @ingroup abstract_format
*/
struct Format_t
{
    const FormatT_Vtable* vtable; /**< vtable */
};


/**
 * @details %FormatT_dtor is an abstract function for the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup abstract_format
*/
inline void
FormatT_dtor(Format_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Format_t));
}


/**
 * @details %FormatT_convert is an abstract function for the convert function.
 *
 * @param   self:       pointer to the class
 * @param   log_info:   pointer to the class Log_info_t
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the convert function
 *
 * @ingroup abstract_format
*/
inline bool
FormatT_convert(Format_t* self, Log_info_t* log_info)
{
    CHECK_SELF(self);

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
 * @ingroup abstract_format
*/
inline void
FormatT_print(Format_t* self)
{
    CHECK_SELF(self);

    self->vtable->print(self);
}

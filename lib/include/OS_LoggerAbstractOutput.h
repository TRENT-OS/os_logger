/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract log format interface
 */
#pragma once

/**
 * @defgroup    abstract_log_output Abstract log server backend interface
 *
 * @brief       Defined abstract interface for log server backend.
 * @details     This level provides an abstract interface for print the log
 *              format to an defined backend.
 *
 *              This interface is abstract, so an implementation is necessary in
 *              a derived layer.
 *              \n
 *              This layer is depend on the layer
 *              \link abstract_observer \endlink, while
 *              it is implementing as observer design pattern.
 *
 *              The interface serves as an observer to a log subject. After
 *              changing a log subject, the objects are updated and output their
 *              data by dropping the log format on the defined backend.
 *              \n
 *              Further it must be implemented the observer functions in the
 *              derived layer.
 *
 * @see         abstract_observer
 *
 * @ingroup     seos_logger_server
*/
#include <stdbool.h>
#include <string.h>
#include "OS_LoggerAbstractObserver.h"
#include "OS_LoggerSymbols.h"


/**
 * @details Output_t defines the class datatype.
 *
 * @ingroup abstract_log_output
*/
typedef struct Output_t Output_t;


/**
 * @details Output_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup abstract_log_output
*/
typedef void
(*Output_dtorT)(Output_t* self);

/**
 * @details Output_printT defines the interface for function pointer to print
 *          the log format to an defined backend.
 *
 * @param   self:   pointer to the class
 * @param   data:   updated log message
 *
 * @return  an status code
 *
 * @ingroup abstract_log_output
*/
typedef bool
(*Output_printT)(Output_t* self, void* data);

/**
 * @details Output_Vtable contain the member functions to his class.
 *          This class is depend on the layers \link abstract_observer \endlink
 *          vtable.
 *
 * @ingroup abstract_log_output
*/
typedef struct
{
    Observer_Vtable parent; /**< parent vtable */
    Output_dtorT    dtor;   /**< function pointer to desctructor */
    Output_printT   print;  /**< function pointer to print function */
}
Output_Vtable;


/**
 * @details Output_t contain the vtable to his class.
 *
 * @ingroup abstract_log_output
*/
struct Output_t
{
    const Output_Vtable* vtable; /**< vtable */
};


/**
 * @details %Output_dtor is an abstract function for the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup abstract_log_output
*/
inline void
Output_dtor(Output_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Output_t));
}


/**
 * @details %Output_print is an abstract function for the print function.
 *
 * @param   self:   pointer to the class
 * @param   data:   updated log message
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the print function
 *
 * @ingroup abstract_log_output
*/
inline bool
Output_print(Output_t* self, void* data)
{
    CHECK_SELF(self);

    return self->vtable->print(self, data);
}

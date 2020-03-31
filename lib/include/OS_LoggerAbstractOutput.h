/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract log format interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractOutput Abstract log server backend interface
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
 * @ingroup     OS_LoggerServer
*/
#include <stdbool.h>
#include <string.h>
#include "OS_LoggerAbstractObserver.h"
#include "OS_LoggerSymbols.h"


/**
 * @details OS_LoggerAbstractOutput_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerAbstractOutput
*/
typedef struct OS_LoggerAbstractOutput_Handle OS_LoggerAbstractOutput_Handle_t;


/**
 * @details OS_LoggerAbstractOutput_dtor_t defines the interface for function
 *          pointer to destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractOutput
*/
typedef void
(*OS_LoggerAbstractOutput_dtor_t)(OS_LoggerAbstractOutput_Handle_t* self);

/**
 * @details OS_LoggerAbstractOutput_print_t defines the interface for function
 *          pointer to print the log format to an defined backend.
 *
 * @param   self:   pointer to the class
 * @param   data:   updated log message
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerAbstractOutput
*/
typedef bool
(*OS_LoggerAbstractOutput_print_t)(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data);

/**
 * @details OS_LoggerAbstractOutput_vtable_t contain the member functions to his
 *          class.
 *
 * This class is dependend on the layers \link abstract_observer \endlink
 * vtable.
 *
 * @ingroup OS_LoggerAbstractOutput
*/
typedef struct
{
    OS_LoggerAbstractObserver_vtable_t parent; //!< parent vtable
    OS_LoggerAbstractOutput_dtor_t   dtor;   //!< function ptr to desctructor
    OS_LoggerAbstractOutput_print_t  print;  //!< function ptr to print
}
OS_LoggerAbstractOutput_vtable_t;


/**
 * @details OS_LoggerAbstractOutput_Handle_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerAbstractOutput
*/
struct OS_LoggerAbstractOutput_Handle
{
    const OS_LoggerAbstractOutput_vtable_t* vtable;
};


/**
 * @details %OS_LoggerAbstractOutput_dtor is an abstract function for the
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractOutput
*/
inline void
OS_LoggerAbstractOutput_dtor(OS_LoggerAbstractOutput_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractOutput_Handle_t));
}


/**
 * @details %OS_LoggerAbstractOutput_print is an abstract function for the print
 *          function.
 *
 * @param   self:   pointer to the class
 * @param   data:   updated log message
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the print function
 *
 * @ingroup OS_LoggerAbstractOutput
*/
inline bool
OS_LoggerAbstractOutput_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    return self->vtable->print(self, data);
}

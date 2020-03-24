/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract subject interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractSubject Abstract subject interface
 *
 * @brief       Defined abstract subject interface.
 * @details     This level provides the mechanism for an abstract subject
 *              interface.
 *              \n
 *              This class provides the provision of subjects that can be
 *              observed by Observer.
 *
 *              The interface functions provide: Registration and deregistration
 *              of observers to specific subjects, and the signaling function
 *              when a subject contains new information that should be
 *              communicated to the Observer in the form of a "notify" function.
 *              \n
 *              The subject's interface allows the observer to register only for
 *              certain events of particular interest. The "attach" function
 *              exists for this. If such an event occurs, the subject only
 *              notifies those observers who have expressed interest in it.
 *
 *              The function "detach" is available to deregister objects.
 *              \n \n
 *              Further it must be implemented the subject functions in the
 *              derived layer.
 *              \n \n
 *              There is a strong dependency to the observer objects.
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
 * @details Subject_t defines the class datatype.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef struct Subject_t Subject_t;


/**
 * @details Subject_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef void
(*Subject_dtorT)(Subject_t* self);


/**
 * @details Subject_attachT defines the interface for function pointer to
 *          register observer object to a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef bool
(*Subject_attachT)(Subject_t* self, Observer_t* observer);


/**
 * @details Subject_detachT defines the interface for function pointer to
 *          deregister observer object from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef bool
(*Subject_detachT)(Subject_t* self, Observer_t* observer);


/**
 * @details Subject_notifyT defines the interface for function pointer to notify
 *          the registraded observer objects.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef void
(*Subject_notifyT)(Subject_t* self, void* data);


/**
 * @details Subject_Vtable contain the member functions to his class.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
typedef struct
{
    Subject_dtorT   dtor;   /**< function pointer to desctructor */
    Subject_attachT attach; /**< function pointer to attach function */
    Subject_detachT detach; /**< function pointer to detach function */
    Subject_notifyT notify; /**< function pointer to notify function */
}
Subject_Vtable;


/**
 * @details Subject_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerAbstractSubject
*/
struct Subject_t
{
    const Subject_Vtable* vtable; /**< vtable */
};


/**
 * @details %Subject_dtor is an abstract function for the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerAbstractSubject
*/
inline void
Subject_dtor(Subject_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Subject_t));
}


/**
 * @details %Subject_attach is an abstract function to register observer object
 *          to a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the print function
 *
 * @ingroup OS_LoggerAbstractSubject
*/
inline bool
Subject_attach(Subject_t* self, Observer_t* observer)
{
    CHECK_SELF(self);

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    return self->vtable->attach(self, observer);
}


/**
 * @details %Subject_detach is an abstract function to deregister observer
 *          object from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to a observer object
 *
 * @return  an status code
 *
 * @retval  depends on the implementation of the print function
 *
 * @ingroup OS_LoggerAbstractSubject
*/
inline bool
Subject_detach(Subject_t* self, Observer_t* observer)
{
    CHECK_SELF(self);

    if (observer == NULL)
    {
        // Debug_printf
        return false;
    }

    return self->vtable->detach(self, observer);
}


/**
 * @details %Subject_notify is an abstract function to notify the registraded
 *          observer objects.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
 *
 * @ingroup OS_LoggerAbstractSubject
*/
inline void
Subject_notify(Subject_t* self, void* data)
{
    CHECK_SELF(self);

    self->vtable->notify(self, data);
}

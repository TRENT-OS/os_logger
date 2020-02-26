/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log subject implementation.
 */
#pragma once

/**
 * @defgroup    log_subject Log subject implemenation
 *
 * @brief       Derived class from class \link abstract_subject \endlink and
 *              provides an implemention for log subject.
 *
 * @details     This level provides an implementation for log subject.
 *              This class is derived by abstract subject.
 *
 *              Concrete observers can be registered to one subject by
 *              calling the "attach" function. Concrete observers can be
 *              deregistraded by calling "detach" function.
 *
 *              To inform the concrete observers by update, call the notify
 *              function.
 *
 *              The concrete implementation is realized as singleton. To one
 *              concrete subject can be registraded to different observers.
 *              There can exist different concrete subjects at the same time,
 *              but the assigning of a concrete observer is only allowed to one
 *              concrete subject at the same time. The assigned from one
 *              concrete observer to two different concrete subject leads
 *              to that the first assignee will be overwritten. And the concrete
 *              observer is assigned to the second (last - if more than two
 *              assigning issues has been done) concrete subject.
 *
 * @ingroup     abstract_subject
*/
#include "abstract_subject.h"
#include <stdbool.h>


/**
 * @details Subject_node_t contain informations about the log subject and is
 *          realized as singleton.
 *
 * @ingroup log_subject
*/
typedef struct
{
    void* first; /**< pointer to the first element */
} Subject_node_t;


/**
 * @details Log_subject_t contain informations about the log subject.
 *
 * @ingroup log_subject
*/
typedef struct
{
    Subject_node_t        node;   /**< contain the singleton object */
    const Subject_Vtable* vtable; /**< vtable */
} Log_subject_t;


/**
 * @details %Log_subject_ctor is the constructor.
 *
 * @param   self:   pointer to the class
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup log_subject
*/
bool
Log_subject_ctor(Log_subject_t* self);


/**
 * @details %Log_subject_ctor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup log_subject
*/
void
Log_subject_dtor(Subject_t* self);


/**
 * @details %Log_subject_attach is for registrade a concrete observer object to
 *          a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to concrete observer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup log_subject
*/
bool
Log_subject_attach(Subject_t* self, Observer_t* observer);


/**
 * @details %Log_subject_detach is for deregistrade a concrete observer object
 *          from a subject.
 *
 * @param   self:       pointer to the class
 * @param   observer:   pointer to concrete observer
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup log_subject
*/
bool
Log_subject_detach(Subject_t* self, Observer_t* observer);


/**
 * @details %Log_subject_notify is for notifying the registraded observer
 *          objects by an update.
 *
 * @param   self:   pointer to the class
 * @param   data:   user data from observer
 *
 * @ingroup log_subject
*/
void
Log_subject_notify(Subject_t* self, void* data);

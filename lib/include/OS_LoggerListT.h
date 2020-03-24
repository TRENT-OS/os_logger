/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief List interface and implementation.
 */
#pragma once


/**
 * @defgroup    OS_LoggerListT List interface and implementation
 *
 * @brief       Helper to the manager list objects.
 * @details     This object provides the interface to deal with lists without
 *              the malloc space.
 *
 * @ingroup     OS_LoggerServer
*/
#include <stdint.h>
#include <stdbool.h>


/**
 * @details ListT_t defines the list datatype.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct ListT_t ListT_t;
/**
 * @details NodeT_t defines the node datatype.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct NodeT_t NodeT_t;


/**
 * @details ListT_dtorT defines the interface for the function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerListT
*/
typedef void
(*ListT_dtorT)(ListT_t* self);


/**
 * @details ListT_hasPrevT defines the interface for the function pointer to
 *          check if the list has the previous object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*ListT_hasPrevT)(NodeT_t* current);


/**
 * @details ListT_hasNextT defines the interface for the function pointer to
 *          check if the list has the next object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*ListT_hasNextT)(NodeT_t* current);


/**
 * @details ListT_getPrevT defines the interface for the function pointer to get
 *          the previous object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*ListT_getPrevT)(NodeT_t* current);


/**
 * @details ListT_getNextT defines the interface for the function pointer to get
 *          the next object from the list.
 *
 * @param   current:    pointer to a list object
 *
 * @return  pointer to list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*ListT_getNextT)(NodeT_t* current);


/**
 * @details ListT_insertT defines the interface for the function pointer to
 *          insert a new object in the list.
 *
 *          The new object will be placed as next object after the current
 *          object.
 *
 * @param   current:    pointer to the list object
 * @param   new_node:   pointer to the list object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*ListT_insertT)(NodeT_t* current, NodeT_t* new_Node);


/**
 * @details ListT_deleteT defines the interface for the function pointer to
 *          delete an object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*ListT_deleteT)(NodeT_t* current);


/**
 * @details ListT_getFirstT defines the interface for the function pointer to
 *          get the first object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*ListT_getFirstT)(NodeT_t* current);


/**
 * @details ListT_getLastT defines the interface for the function pointer to get
 *          the last object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*ListT_getLastT)(NodeT_t* current);


/**
 * @details ListT_isInsideT defines the interface for function pointer to check
 *          if the current object is inserted in the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  the status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*ListT_isInsideT)(NodeT_t* current);


/**
 * @details ListT_Vtable contain the member functions to his class.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct
{
    ListT_dtorT     dtor;      /**< function pointer to desctructor */
    ListT_hasPrevT  has_prev;  /**< function pointer to has previous function */
    ListT_hasNextT  has_next;  /**< function pointer to has next function */
    ListT_getPrevT  get_prev;  /**< function pointer to get previous function */
    ListT_getNextT  get_next;  /**< function pointer to get next function */
    ListT_insertT   insert;    /**< function pointer to insert function */
    ListT_deleteT   delete;    /**< function pointer to delete function */
    ListT_getFirstT get_first; /**< function pointer to get first function */
    ListT_getLastT  get_last;  /**< function pointer to get last function */
    ListT_isInsideT isInside;  /**< function pointer to is inside function */
}
ListT_Vtable;


/**
 * @details ListT_t contains the class' vtable.
 *
 * @ingroup OS_LoggerListT
*/
struct ListT_t
{
    const ListT_Vtable* vtable; /**< virtual table */
};


/**
 * @details NodeT_t contains the node's previous and next objects.
 *
 * @ingroup OS_LoggerListT
*/
struct NodeT_t
{
    void* prev; /**< (pointer) node to previous object */
    void* next; /**< (pointer) node to next object */
};


/**
 * @details %ListT_ctor is the constructor.
 *
 * @param   self:   pointer to the class
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup OS_LoggerListT
*/
bool
ListT_ctor(ListT_t* self);


/**
 * @details %ListT_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerListT
*/
void
ListT_dtor(ListT_t* self);

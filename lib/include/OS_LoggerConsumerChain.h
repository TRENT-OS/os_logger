/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Consumer chain implementation
 */
#pragma once

/**
 * @defgroup    consumer_chain Consumer chain implementation
 *
 * @brief       This layer provides to manage consumer log objects.
 * @details     All log consumer objects, which will be getting logged, has to
 *              append in a logger chain.
 *              This layer provides functions to append or remove consumer log
 *              objects into or from this consumer chain.
 *              \n
 *              The poll process is started with the poll function. At this
 *              point, all objects in the consumer chain must be attached.
 *              \n \n
 *              Using the "get sender id" function, a log consumer, i.e. a log
 *              client, can be clearly identified when a log message arrives.
 *              This implementation works as a singleton. That means that only
 *              one consumer log chain can exist at a time.
 *              \n \n
 *              This layer is implemented as singleton.
 *
 * @ingroup     seos_logger_server
*/
#include "OS_LoggerConsumer.h"
#include "OS_LoggerSymbols.h"
#include <stdbool.h>


/**
 * @details Consumer_chain_t defines the class datatype.
 *
 * @ingroup consumer_chain
*/
typedef struct Consumer_chain_t Consumer_chain_t;


/**
 * @details Consumer_chain_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @ingroup consumer_chain
*/
typedef void
(*Consumer_chain_dtorT)(void);


/**
 * @details Consumer_chain_appendT defines the interface for function pointer to
 *          append a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @ingroup consumer_chain
*/
typedef bool
(*Consumer_chain_appendT)(Log_consumer_t* consumer);


/**
 * @details Consumer_chain_removeT defines the interface for function pointer to
 *          remove a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @ingroup consumer_chain
*/
typedef bool
(*Consumer_chain_removeT)(Log_consumer_t* consumer);


/**
 * @details Consumer_chain_get_senderT defines the interface for function
 *          pointer to determine a calling consumer log object by sender id.
 *
 * @return  pointer to consumer log object
 *
 * @ingroup consumer_chain
*/
typedef Log_consumer_t*
(*Consumer_chain_get_senderT)(void);


/**
 * @details Consumer_chain_pollT defines the interface for function pointer to
 *          start the poll process.
 *
 * @ingroup consumer_chain
*/
typedef void
(*Consumer_chain_pollT)(void);


/**
 * @details Consumer_chain_Vtable contain the member functions to his class.
 *
 * @ingroup consumer_chain
*/
typedef struct
{
    Consumer_chain_dtorT       dtor;
    Consumer_chain_appendT     append;
    Consumer_chain_removeT     remove;
    Consumer_chain_get_senderT get_sender;
    Consumer_chain_pollT       poll;
}
Consumer_chain_Vtable;


/**
 * @details Consumer_chain_node_t contain informations about the consumer log
 *          object and is realized as singleton.
 *
 * @ingroup consumer_chain
*/
typedef struct
{
    void* first; /**< pointer to the first element */
} Consumer_chain_node_t;


/**
 * @details Consumer_chain_t contain the vtable to his class.
 *
 * @ingroup consumer_chain
*/
struct Consumer_chain_t
{
    ListT_t                     listT;
    Consumer_chain_node_t       node;
    const Consumer_chain_Vtable* vtable;
};


/**
 * @details %get_instance_Consumer_chain is a getter to instantiate this object.
 *          This is an singleton implementation. It will internally create a
 *          consumer chain object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, in the other case this functions returns a
 *          NULL pointer.
 *
 * @return  pointer to Consumer_chain_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup consumer_chain
*/
Consumer_chain_t*
get_instance_Consumer_chain(void);


/**
 * @details %Consumer_chain_dtor is the destructor.
 *
 * @ingroup consumer_chain
*/
void
Consumer_chain_dtor(void);


/**
 * @details %Consumer_chain_append provides to append a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup consumer_chain
*/
bool
Consumer_chain_append(Log_consumer_t* consumer);


/**
 * @details %Consumer_chain_remove provides to remove a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup consumer_chain
*/
bool
Consumer_chain_remove(Log_consumer_t* consumer);


/**
 * @details %Consumer_chain_get_sender provides to determine a calling consumer
 *          log object by sender id.
 *
 * @return  pointer to log consumer object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup consumer_chain
*/
Log_consumer_t*
Consumer_chain_get_sender(void);


/**
 * @details %Consumer_chain_poll provides to start the poll process.
 *
 * @ingroup consumer_chain
*/
void
Consumer_chain_poll(void);

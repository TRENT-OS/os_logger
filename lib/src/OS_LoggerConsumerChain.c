/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerConsumerChain.h"
#include <string.h>



static const OS_LoggerConsumerChain_vtable_t Consumer_chain_vtable =
{
    .dtor                     = OS_LoggerConsumerChain_dtor,
    .append                   = OS_LoggerConsumerChain_append,
    .remove                   = OS_LoggerConsumerChain_remove,
    .get_sender               = OS_LoggerConsumerChain_getSender,
    .poll                     = OS_LoggerConsumerChain_poll
};



// Singleton
static OS_LoggerConsumerChain_Handle_t _consumer_chain;
static OS_LoggerConsumerChain_Handle_t* this = NULL;



OS_LoggerConsumerChain_Handle_t*
OS_LoggerConsumerChain_getInstance(void)
{
    if (this == NULL)
    {
        bool retval = false;

        this = &_consumer_chain;
        this->vtable = &Consumer_chain_vtable;

        retval = OS_LoggerListT_ctor(&this->listT);
        if (retval == false)
        {
            return NULL;
        }
    }

    return this;
}



void
OS_LoggerConsumerChain_dtor(void)
{
    memset(this, 0, sizeof (OS_LoggerConsumerChain_Handle_t));
    this = NULL;
}



bool
OS_LoggerConsumerChain_append(OS_LoggerConsumer_Handle_t* consumer)
{
    OS_Logger_CHECK_SELF(this);

    bool retval = false;

    if (consumer == NULL)
    {
        // Debug_printf
        return retval;
    }

    if (this->node.first == NULL)
    {
        this->node.first = consumer;
        return true;
    }

    retval = this->listT.vtable->insert(
                 this->listT.vtable->get_last(
                     (OS_LoggerNodeT_Handle_t*)
                     & (((OS_LoggerConsumer_Handle_t*)(this->node.first))->node)),
                 &consumer->node);

    return retval;
}



bool
OS_LoggerConsumerChain_remove(OS_LoggerConsumer_Handle_t* consumer)
{
    OS_Logger_CHECK_SELF(this);

    bool retval = false;

    if (consumer == NULL)
    {
        // Debug_printf
        return false;
    }

    if (this->node.first == consumer)
    {
        this->node.first = this->listT.vtable->get_next(&consumer->node);

        if (this->node.first == consumer)
        {
            this->node.first = NULL;
        }
    }

    retval = this->listT.vtable->delete (&consumer->node);

    return retval;
}



void
OS_LoggerConsumerChain_poll(void)
{
    OS_Logger_CHECK_SELF(this);

    OS_LoggerConsumer_Handle_t* log_consumer;

    log_consumer = this->node.first;
    log_consumer->vtable->emit(log_consumer);
}



OS_LoggerConsumer_Handle_t*
OS_LoggerConsumerChain_getSender(void)
{
    OS_Logger_CHECK_SELF(this);

    OS_LoggerConsumer_Handle_t* log_consumer;

    if (this->node.first == NULL)
    {
        return NULL;
    }

    log_consumer = this->node.first;

    do
    {
        if (log_consumer->id == log_consumer->callback_vtable->get_sender_id())
        {
            return log_consumer;
            break;
        }
    }
    while ( (log_consumer = this->listT.vtable->get_next(&log_consumer->node)) !=
            NULL );

    return NULL;
}



void
API_LOG_SERVER_EMIT(void)
{
    OS_Logger_CHECK_SELF(this);

    OS_LoggerConsumer_Handle_t* log_consumer;

    log_consumer = this->vtable->get_sender();
    if (log_consumer == NULL)
    {
        // Debug_printf
        return;
    }

    log_consumer->vtable->process(log_consumer);

    ((OS_LoggerConsumer_Handle_t*)(this->node.first))->vtable->emit(
        (OS_LoggerConsumer_Handle_t*)(this->node.first));
}

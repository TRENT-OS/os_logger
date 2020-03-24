/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "OS_LoggerConsumerChain.h"
#include <string.h>



static const Consumer_chain_Vtable Consumer_chain_vtable =
{
    .dtor                     = Consumer_chain_dtor,
    .append                   = Consumer_chain_append,
    .remove                   = Consumer_chain_remove,
    .get_sender               = Consumer_chain_get_sender,
    .poll                     = Consumer_chain_poll
};



// Singleton
static Consumer_chain_t _consumer_chain;
static Consumer_chain_t* this = NULL;



Consumer_chain_t*
get_instance_Consumer_chain(void)
{
    if (this == NULL)
    {
        bool retval = false;

        this = &_consumer_chain;
        this->vtable = &Consumer_chain_vtable;

        retval = ListT_ctor(&this->listT);
        if (retval == false)
        {
            return NULL;
        }
    }

    return this;
}



void
Consumer_chain_dtor(void)
{
    memset(this, 0, sizeof (Consumer_chain_t));
    this = NULL;
}



bool
Consumer_chain_append(Log_consumer_t* consumer)
{
    CHECK_SELF(this);

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

    retval = this->listT.vtable->insert(this->listT.vtable->get_last((
                                            NodeT_t*) & (((Log_consumer_t*)(this->node.first))->node)), &consumer->node);

    return retval;
}



bool
Consumer_chain_remove(Log_consumer_t* consumer)
{
    CHECK_SELF(this);

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
Consumer_chain_poll(void)
{
    CHECK_SELF(this);

    Log_consumer_t* log_consumer;

    log_consumer = this->node.first;
    log_consumer->vtable->emit(log_consumer);
}



Log_consumer_t*
Consumer_chain_get_sender(void)
{
    CHECK_SELF(this);

    Log_consumer_t* log_consumer;

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
    CHECK_SELF(this);

    Log_consumer_t* log_consumer;

    log_consumer = this->vtable->get_sender();
    if (log_consumer == NULL)
    {
        // Debug_printf
        return;
    }

    log_consumer->vtable->process(log_consumer);

    ((Log_consumer_t*)(this->node.first))->vtable->emit((Log_consumer_t*)(
                                                            this->node.first));
}
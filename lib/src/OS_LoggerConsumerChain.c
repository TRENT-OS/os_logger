/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerConsumerChain.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>

static const OS_LoggerConsumerChain_vtable_t Consumer_chain_vtable =
{
    .append                   = OS_LoggerConsumerChain_append,
    .remove                   = OS_LoggerConsumerChain_remove,
    .get_sender               = OS_LoggerConsumerChain_getSender,
};


// Singleton
static OS_LoggerConsumerChain_Handle_t _consumer_chain;
static OS_LoggerConsumerChain_Handle_t* this = NULL;



OS_LoggerConsumerChain_Handle_t*
OS_LoggerConsumerChain_getInstance(void)
{
    if (this == NULL)
    {
        this = &_consumer_chain;
        this->vtable = &Consumer_chain_vtable;
    }

    return this;
}

OS_Error_t
OS_LoggerConsumerChain_append(OS_LoggerConsumer_Handle_t* consumer)
{
    OS_Logger_CHECK_SELF(this);

    if (consumer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (this->node.first == NULL)
    {
        this->node.first = consumer;
        return OS_SUCCESS;
    }

    OS_LoggerListT_insert(
        OS_LoggerListT_getLast(
            &(((OS_LoggerConsumer_Handle_t*)(this->node.first))->node)),
        &consumer->node);

    return OS_SUCCESS;
}



OS_Error_t
OS_LoggerConsumerChain_remove(OS_LoggerConsumer_Handle_t* consumer)
{
    OS_Logger_CHECK_SELF(this);

    if (consumer == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (this->node.first == consumer)
    {
        this->node.first = OS_LoggerListT_getNext(&consumer->node);

        if (this->node.first == consumer)
        {
            this->node.first = NULL;
        }
    }

    OS_LoggerListT_erase(&consumer->node);

    return OS_SUCCESS;
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
        if (log_consumer->entry->consumerMetadata.id
            == log_consumer->callback_vtable->get_sender_id())
        {
            return log_consumer;
            break;
        }
    }
    while ((log_consumer = OS_LoggerListT_getNext(
                               &log_consumer->node)) != NULL);

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
        return;
    }

    log_consumer->vtable->process(log_consumer);
}

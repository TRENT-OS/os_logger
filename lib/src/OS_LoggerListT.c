/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "Logger/Server/OS_LoggerListT.h"

#include <stddef.h>

bool
OS_LoggerListT_hasPrevious(OS_LoggerNodeT_Handle_t* current)
{
    bool retval = false;

    if (current == NULL)
    {
        return retval;
    }

    if (current->prev != NULL)
    {
        retval = true;
    }

    return retval;
}

bool
OS_LoggerListT_hasNext(OS_LoggerNodeT_Handle_t* current)
{
    bool retval = false;

    if (current == NULL)
    {
        return retval;
    }

    if (current->next != NULL)
    {
        retval = true;
    }

    return retval;
}

void*
OS_LoggerListT_getPrevious(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* prev = NULL;

    if (current == NULL)
    {
        return prev;
    }

    if (OS_LoggerListT_hasPrevious(current))
    {
        prev = current->prev;
        return prev;
    }

    return prev;
}

void*
OS_LoggerListT_getNext(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* next = NULL;

    if (current == NULL)
    {
        return next;
    }

    if (OS_LoggerListT_hasNext(current))
    {
        next = current->next;
        return next;
    }

    return next;
}

void*
OS_LoggerListT_insert(
    OS_LoggerNodeT_Handle_t* current,
    OS_LoggerNodeT_Handle_t* newNode)
{
    OS_LoggerNodeT_Handle_t* next = NULL;

    if ((      NULL == current)
        ||    (NULL == newNode)
        || (current == newNode))
    {
        return newNode;
    }

    if (OS_LoggerListT_hasNext(current))
    {
        next = OS_LoggerListT_getNext(current);
        next->prev = newNode;
    }

    current->next = newNode;

    newNode->prev = current;
    newNode->next = next;

    return newNode;
}

void*
OS_LoggerListT_erase(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* prev = NULL;
    OS_LoggerNodeT_Handle_t* next = NULL;

    if (current == NULL)
    {
        return NULL;
    }

    if (OS_LoggerListT_hasPrevious(current))
    {
        prev = OS_LoggerListT_getPrevious(current);
    }

    if (OS_LoggerListT_hasNext(current))
    {
        next = OS_LoggerListT_getNext(current);
    }

    if (next == NULL && prev == NULL)
    {
        return NULL;
    }
    else if (next == NULL)
    {
        prev->next = next;
    }
    else if (prev == NULL)
    {
        next->prev = prev;
    }
    else
    {
        prev->next = next;
        next->prev = prev;
    }

    current->prev = NULL;
    current->next = NULL;

    return next;
}

void*
OS_LoggerListT_getFirst(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* first = NULL;

    if (current == NULL)
    {
        return first;
    }

    while (OS_LoggerListT_hasPrevious(current))
    {
        first = OS_LoggerListT_getPrevious(current);
        current = first;
    }

    if (first == NULL)
    {
        first = current;
    }

    return first;
}

void*
OS_LoggerListT_getLast(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* last = NULL;

    if (current == NULL)
    {
        return last;
    }

    while (OS_LoggerListT_hasNext(current))
    {
        last = OS_LoggerListT_getNext(current);
        current = last;
    }

    if (last == NULL)
    {
        last = current;
    }

    return last;
}

bool
OS_LoggerListT_isInside(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* first = NULL;
    bool retval = false;

    if (current == NULL)
    {
        return retval;
    }

    while (OS_LoggerListT_hasNext(first))
    {
        if (first == current)
        {
            retval = true;
            break;
        }

        first = OS_LoggerListT_getNext(first);
    }

    if (first == current)
    {
        retval = true;
    }

    return retval;
}

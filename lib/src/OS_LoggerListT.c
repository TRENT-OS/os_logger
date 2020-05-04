/* Copyright (C) 2020, HENSOLDT Cyber GmbH */
#include "Logger/Server/OS_LoggerListT.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <string.h>
#include <stddef.h>



static bool  _ListT_has_prev(OS_LoggerNodeT_Handle_t* current);
static bool  _ListT_has_next(OS_LoggerNodeT_Handle_t* current);
static void* _ListT_get_prev(OS_LoggerNodeT_Handle_t* current);
static void* _ListT_get_next(OS_LoggerNodeT_Handle_t* current);

static
seos_err_t
_ListT_insert(
    OS_LoggerNodeT_Handle_t* current,
    OS_LoggerNodeT_Handle_t* new_node);

static seos_err_t   _ListT_delete(OS_LoggerNodeT_Handle_t* current);
static void*        _ListT_get_first(OS_LoggerNodeT_Handle_t* current);
static void*        _ListT_get_last(OS_LoggerNodeT_Handle_t* current);
static bool         _ListT_isInside(OS_LoggerNodeT_Handle_t* current);



static const OS_LoggerListT_vtable_t ListT_vtable =
{
    .dtor      = OS_LoggerListT_dtor,
    .has_prev  = _ListT_has_prev,
    .has_next  = _ListT_has_next,
    .get_prev  = _ListT_get_prev,
    .get_next  = _ListT_get_next,
    .insert    = _ListT_insert,
    .delete    = _ListT_delete,
    .get_first = _ListT_get_first,
    .get_last  = _ListT_get_last,
    .isInside  = _ListT_isInside
};



void
OS_LoggerListT_ctor(OS_LoggerListT_t_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    self->vtable = &ListT_vtable;
}



void
OS_LoggerListT_dtor(OS_LoggerListT_t_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerListT_t_Handle_t));
}



static bool
_ListT_has_prev(OS_LoggerNodeT_Handle_t* current)
{
    bool retval = false;

    if (current == NULL)
    {
        // Debug_printf
        return retval;
    }

    if (current->prev != NULL)
    {
        retval = true;
    }

    return retval;
}



static bool
_ListT_has_next(OS_LoggerNodeT_Handle_t* current)
{
    bool retval = false;

    if (current == NULL)
    {
        // Debug_printf
        return retval;
    }

    if (current->next != NULL)
    {
        retval = true;
    }

    return retval;
}




static void*
_ListT_get_prev(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* prev = NULL;

    if (current == NULL)
    {
        // Debug_printf
        return prev;
    }

    if (_ListT_has_prev(current))
    {
        prev = current->prev;
        return prev;
    }

    return prev;
}




static void*
_ListT_get_next(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* next = NULL;

    if (current == NULL)
    {
        // Debug_printf
        return next;
    }

    if (_ListT_has_next(current))
    {
        next = current->next;
        return next;
    }

    return next;
}




static
seos_err_t
_ListT_insert(
    OS_LoggerNodeT_Handle_t* current,
    OS_LoggerNodeT_Handle_t* new_node)
{
    OS_LoggerNodeT_Handle_t* next = NULL;

    if ((current == NULL) || (new_node == NULL))
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (current == new_node)
    {
        return SEOS_SUCCESS;
    }

    if (_ListT_has_next(current))
    {
        next = _ListT_get_next(current);
        next->prev = new_node;
    }

    current->next = new_node;

    new_node->prev = current;
    new_node->next = next;

    return SEOS_SUCCESS;
}




static
seos_err_t
_ListT_delete(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* prev = NULL;
    OS_LoggerNodeT_Handle_t* next = NULL;

    if (current == NULL)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (_ListT_has_prev(current))
    {
        prev = _ListT_get_prev(current);
    }

    if (_ListT_has_next(current))
    {
        next = _ListT_get_next(current);
    }

    if (next == NULL && prev == NULL)
    {
        return SEOS_SUCCESS;
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

    return SEOS_SUCCESS;
}



static void*
_ListT_get_first(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* first = NULL;

    if (current == NULL)
    {
        // Debug_printf
        return first;
    }

    while (_ListT_has_prev(current))
    {
        first = _ListT_get_prev(current);
        current = first;
    }

    if (first == NULL)
    {
        first = current;
    }

    return first;
}



static void*
_ListT_get_last(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* last = NULL;

    if (current == NULL)
    {
        // Debug_printf
        return last;
    }

    while (_ListT_has_next(current))
    {
        last = _ListT_get_next(current);
        current = last;
    }

    if (last == NULL)
    {
        last = current;
    }

    return last;
}



static bool
_ListT_isInside(OS_LoggerNodeT_Handle_t* current)
{
    OS_LoggerNodeT_Handle_t* first = NULL;
    bool retval = false;

    if (current == NULL)
    {
        // Debug_printf
        return retval;
    }

    while (_ListT_has_next(first))
    {
        if (first == current)
        {
            retval = true;
            break;
        }

        first = _ListT_get_next(first);
    }

    if (first == current)
    {
        retval = true;
    }

    return retval;
}

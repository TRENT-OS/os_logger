#pragma once


#include "abstract_log_output.h"
#include "log_format.h"
#include "listT.h"


typedef struct
{
    NodeT_t             node;
    ListT_t             listT;
    Log_format_t*        log_format;
    const Output_Vtable* vtable;
} Log_output_t;

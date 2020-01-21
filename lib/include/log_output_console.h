#pragma once


#include "log_output.h"


bool
Log_output_console_ctor(Log_output_t* self,
                        Log_format_t* log_format);


void
Log_output_console_dtor(Output_t* self);

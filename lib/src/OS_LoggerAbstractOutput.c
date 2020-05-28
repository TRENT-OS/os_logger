#include "Logger/Server/OS_LoggerAbstractOutput.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include <stdbool.h>
#include <string.h>

void
OS_LoggerAbstractOutput_dtor(OS_LoggerAbstractOutput_Handle_t* self)
{
    OS_Logger_CHECK_SELF(self);

    memset(self, 0, sizeof (OS_LoggerAbstractOutput_Handle_t));
}

OS_Error_t
OS_LoggerAbstractOutput_print(
    OS_LoggerAbstractOutput_Handle_t* self,
    void* data)
{
    OS_Logger_CHECK_SELF(self);

    return self->vtable->print(self, data);
}

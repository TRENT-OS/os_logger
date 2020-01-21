#pragma once


#include <stdbool.h>
#include <string.h>
#include "log_symbol.h"


typedef struct Observer_t Observer_t;


typedef void
(*Observer_dtorT)(Observer_t* self);


typedef bool
(*Observer_updateT)(Observer_t* self, void* data);


typedef struct
{
    Observer_dtorT   dtor;
    Observer_updateT update;
}
Observer_Vtable;


struct Observer_t
{
    const Observer_Vtable* vtable;
};


inline void
Observer_dtor(Observer_t* self)
{
    CHECK_SELF(self);

    memset(self, 0, sizeof (Observer_t));
}


inline bool
Observer_update(Observer_t* self, void* data)
{
    CHECK_SELF(self);

    self->vtable->update(self, data);

    return true;
}

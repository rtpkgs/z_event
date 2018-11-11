#include "z_event.h"
#include <stdlib.h>
#include "stdarg.h"
#include "string.h"

struct event
{
    uint64_t * event;
    uint32_t length;
};
typedef struct event * event_t;

hz_event_t z_event_create(uint32_t length)
{
    event_t event = (event_t)malloc(sizeof(struct event));
    memset(event, 0, sizeof(struct event));
    if(length % 64 == 0)
    {
        event->event = (uint64_t *)malloc(length / 8);
    }
    else
    {
        event->event = (uint64_t *)malloc((length / 64 + 1) * 8);
    }
    event->length = length;
    z_event_clean(event);
    return (hz_event_t)event;
}

z_event_result z_event_delete(hz_event_t event)
{
    event_t _event = (event_t)event;
    free(_event->event);
    free(_event);
    return EVENT_OK;
}

z_event_result z_event_set(hz_event_t event, uint32_t num_event,...)
{
    va_list args;
    uint32_t id, i;
    event_t _event = (event_t)event;
    va_start(args, num_event);
    for(i = 0; i < num_event; i++)
    {
        id = va_arg(args, int);
        if(id > _event->length)
        {
            va_end(args);
            return EVENT_ERROR;
        }
        _event->event[id / 64] |= (uint64_t)0x01 << (id % 64);
    }
    va_end(args);
    return EVENT_OK;
}



z_event_result z_event_reset(hz_event_t event, uint32_t num_event, ...)
{
    va_list args;
    uint32_t id, i;
    event_t _event = (event_t)event;
    va_start(args, num_event);
    for(i = 0; i < num_event; i++)
    {
        id = va_arg(args, int);
        if(id > _event->length)
        {
            va_end(args);
            return EVENT_ERROR;
        }
        _event->event[id / 64] &= ~((uint64_t)0x01 << (id % 64));
    }
    va_end(args);
    return EVENT_OK;
}

z_event_result z_event_clean(hz_event_t event)
{
    uint8_t length, i;
    event_t _event = (event_t)event;
    if(_event->length % 64 == 0)
    {
        length = _event->length / 64;
    }
    else
    {
        length = _event->length / 64 + 1;
    }
    for(i = 0; i < length; i++)
    {
        _event->event[i] = 0;
    }
    return EVENT_OK;
}

z_event_result z_event_is_all_happen(hz_event_t event, uint32_t num_event, ...)
{
    va_list args;
    uint32_t id, i;
    event_t _event = (event_t)event;
    va_start(args, num_event);
    for(i = 0; i < num_event; i++)
    {
        id = va_arg(args, int);
        if(id > _event->length)
        {
            va_end(args);
            return EVENT_ERROR;
        }
        
        if((_event->event[id / 64] & ((uint64_t)0x01 << (id % 64))) == 0)
        {
            return EVENT_FALSE;
        }
    }
    va_end(args);
    return EVENT_TRUE;
}

z_event_result z_event_is_one_of_happen(hz_event_t event, uint32_t num_event, ...)
{
    va_list args;
    uint32_t id, i;
    event_t _event = (event_t)event;
    va_start(args, num_event);
    for(i = 0; i < num_event; i++)
    {
        id = va_arg(args, int);
        if(id > _event->length)
        {
            va_end(args);
            return EVENT_ERROR;
        }

        if((_event->event[id / 64] & ((uint64_t)0x01 << (id % 64))) != 0)
        {
            return EVENT_TRUE;
        }
    }
    va_end(args);
    return EVENT_FALSE;
}

z_event_result z_event_is_none_happen(hz_event_t event)
{
    uint8_t length, i;
    event_t _event = (event_t)event;
    if(_event->length % 64 == 0)
    {
        length = _event->length / 64;
    }
    else
    {
        length = _event->length / 64 + 1;
    }
    for(i = 0; i < length; i++)
    {
        if(_event->event[i] > 0)
            return EVENT_FALSE;
    }
    return EVENT_TRUE;
}

void z_event_copy(hz_event_t source, hz_event_t destin)
{
    uint8_t length , i;
    event_t _source = (event_t)source;
    event_t _destin = (event_t)destin;
    if(_source->length % 64 == 0)
    {
        length = _source->length / 64;
    }
    else
    {
        length = _source->length / 64 + 1;
    }
    for(i = 0; i < length; i++)
    {
        _destin->event[i] = _source->event[i];
    }
}

hz_event_t z_event_dup(hz_event_t event)
{
    event_t _source = (event_t)event;
    hz_event_t dst = z_event_create(_source->length);
    z_event_copy(event, dst);
    return dst;
}




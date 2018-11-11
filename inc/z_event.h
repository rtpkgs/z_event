#ifndef __Z_EVENT_H__
#define __Z_EVENT_H__
#include <stdint.h>

#if defined(__cplusplus)||defined(c_plusplus)
extern "C"{
#endif

typedef enum 
{
    EVENT_OK,
    EVENT_ERROR,
    EVENT_FALSE,
    EVENT_TRUE
} z_event_result;

typedef void * hz_event_t;

extern hz_event_t       z_event_create              (uint32_t   length);
extern z_event_result   z_event_delete              (hz_event_t event);
extern z_event_result   z_event_set                 (hz_event_t event, uint32_t num_event, ...);
extern z_event_result   z_event_reset               (hz_event_t event, uint32_t num_event, ...);
extern z_event_result   z_event_clean               (hz_event_t event);
extern z_event_result   z_event_is_all_happen       (hz_event_t event, uint32_t num_event, ...);
extern z_event_result   z_event_is_one_of_happen    (hz_event_t event, uint32_t num_event, ...);
extern z_event_result   z_event_is_none_happen      (hz_event_t event);
extern void             z_event_copy                (hz_event_t source, hz_event_t destin);
extern hz_event_t       z_event_dup                 (hz_event_t event);

#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif


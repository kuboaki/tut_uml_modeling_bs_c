#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdbool.h>

/* ---------- State ---------- */

typedef enum {
    ST0,
    ST1,
    ST2,
    ST_INVALID
} State;

const char *state_name(State s);

/* ---------- SampleState ---------- */

typedef struct {
    State current;
} SampleState;

void  sample_state_init(SampleState *ss);
State sample_state_transit_to(SampleState *ss, State new_state);

/* ---------- Event ---------- */

typedef enum {
    EV1,
    EV2,
    EV3,
    EV_UNKNOWN
} Event;

const char *event_name(Event e);

/* ---------- Sample ---------- */

typedef struct {
    SampleState state;
    bool guards[2];
} Sample;

void sample_init(Sample *s);
void sample_play(Sample *s, Event evt, int p01);

#endif /* SAMPLE_H */

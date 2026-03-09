#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdbool.h>

typedef enum {
    ST0, // <1>
    ST1,
    ST2,
    ST_INVALID
} State; // <2>

typedef struct {
    State current;
} SampleState; // <3>

void  sample_state_init(SampleState *ss); // <4>

typedef enum {
    EV1, // <1>
    EV2,
    EV3,
    EV_UNKNOWN
} Event;  // <2>

typedef struct {
    bool attr_a;
    bool attr_b;
    SampleState state;
} Sample;   // <1>

void sample_init(Sample *s);
void sample_play(Sample *s, Event evt, int p01); // <1>

#endif /* SAMPLE_H */

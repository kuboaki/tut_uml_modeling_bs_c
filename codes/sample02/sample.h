#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    ST0,  // <1>
    ST1,
    ST2
} SampleState;  // <2>

typedef enum {
    EV1,  // <1>
    EV2,
    EV3
} SampleEvent;  // <2>

typedef struct {
    bool attr_a;
    bool attr_b;
    SampleState state;  // <1>
} Sample;

void sample_init(Sample *sample);
void sample_play(Sample *sample, SampleEvent evt, int p01); // <1>

#endif /* SAMPLE_H */

#include "sample.h" // <1>

int main(void) {
    Sample smpl01;               // <2>
    sample_init(&smpl01);        // <3>
    SampleEvent evt = EV1;
    sample_play(&smpl01, evt, 1); // <4>
}

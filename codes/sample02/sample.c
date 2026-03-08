#include "sample.h" // <1>

void sample_init(Sample *sample) {
    sample->attr_a = true;
    sample->attr_b = true;
    sample->state = ST0; // <2>
}

static bool gd1(Sample *sample) {
    return sample->attr_a && sample->attr_b;
}

static bool gd2(Sample *sample) {
    return !sample->attr_a || sample->attr_b;
}

static void act1(Sample *sample, SampleEvent evt, int prm) {
    printf("    act1: state: %d, event:%d, param: %d\n", sample->state, evt, prm);
}

static void act2(Sample *sample, SampleEvent evt, int prm) {
    printf("    act2: state: %d, event:%d, param: %d\n", sample->state, evt, prm);
}

static void act3(Sample *sample, SampleEvent evt, int prm) {
    printf("    act3: state: %d, event:%d, param: %d\n", sample->state, evt, prm);
}

static void st0_proc(Sample *sample, SampleEvent evt, int param) {
    switch (evt) {  // <1>
        case EV1:  // <2>
            if (gd1(sample)) {  // <3>
                act1(sample, evt, param);  // <4>
                sample->state = ST1;  // <5>
            } else {
                printf("Event ignored due to guard condition\n");  // <6>
            }
            break;
        case EV3:  // <7>
            act3(sample, evt, param);
            sample->state = ST0;
            break;
        default:
            /* その他のイベントは無視 */
            break;
    }
}

static void st1_proc(Sample *sample, SampleEvent evt, int param) {
    switch (evt) {  // <1>
        case EV2:  // <2>
            act2(sample, evt, param);  // <4>
            if (gd2(sample)) {  // <3>
                act3(sample, evt, param);  // <4>
                sample->state = ST0;  // <5>
            } else {
                sample->state = ST2;  // <6>
            }
            break;
        case EV3:  // <7>
            act3(sample, evt, param);
            sample->state = ST0;
            break;
        default:
            /* その他のイベントは無視 */
            break;
    }
}

void sample_play(Sample *sample, SampleEvent evt, int param) { // <1>
    switch (sample->state) {  // <2>
        case ST0:  // <3>
            st0_proc(sample, evt, param);  // <4>
            break;
        case ST1:
            st1_proc(sample, evt, param);
            break;
        case ST2:
            // none
            break;
    }
}

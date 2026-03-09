#include <stdio.h>
#include "sample.h" // <1>

void sample_init(Sample *s) {
    s->state.current = ST0; // <2>
    s->attr_a = true;
    s->attr_b = true;
}

static bool gd1(const Sample *s) { // <1>
    return s->attr_a && s->attr_b; // <2>
}

static bool gd2(const Sample *s) { // <3>
    return !s->attr_a || s->attr_b;
}

static void act1(Event evt, int prm) { // <1>
    printf("    act1: event:%d, param: %d\n", evt, prm);
}

static void act2(Event evt, int prm) { // <2>
    printf("    act2: event:%d, param: %d\n", evt, prm);
}

static void act3(Event evt, int prm) { // <3>
    printf("    act3: event:%d, param: %d\n", evt, prm);
}

static void st0_proc(Sample *s, Event evt, int p01) {
    switch (evt) {  // <1>
        case EV1:  // <2>
            if (gd1(s)) {  // <3>
                act1(evt, p01);  // <4>
                s->state.current = ST1;  // <5>
            } else {
                printf("Event ignored due to guard condition\n");  // <6>
            }
            break;
        case EV3:  // <7>
            act3(evt, p01);
            s->state.current = ST0;
            break;
        default:
            /* その他のイベントは無視 */
            break;
    }
}

static void st1_proc(Sample *s, Event evt, int p01) {
    switch (evt) {  // <1>
    case EV2:  // <2>
        act2(evt, p01);  // <4>
        if (gd2(s)) {  // <3>
            act3(evt, p01);  // <4>
            s->state.current = ST0;  // <5>
        } else {
            s->state.current = ST2;  // <6>
        }
        break;
    case EV3:  // <7>
        act3(evt, p01);
        s->state.current = ST0;
        break;
    default:
        /* その他のイベントは無視 */
        break;
    }
}

void sample_play(Sample *s, Event evt, int p01) { // <1>
    switch (s->state.current) {  // <2>
    case ST0:  // <3>
        st0_proc(s, evt, p01);  // <4>
        break;
    case ST1:
        st1_proc(s, evt, p01);
        break;
    case ST2:
        // none
        break;
    case ST_INVALID:
        printf("invalid state: %d.\n", s->state.current);
        break;
    }
}

#include <stdio.h>
#include "sample.h"

/* ---------- State ---------- */

const char *state_name(State s) { // <1>
    switch (s) {
        case ST0: return "ST0";
        case ST1: return "ST1";
        case ST2: return "ST2";
        default:  return "UNKNOWN";
    }
}

/* ---------- SampleState ---------- */

void sample_state_init(SampleState *ss) {
    ss->current = ST0;
}

State sample_state_transit_to(SampleState *ss, State new_state) {
    if (new_state < ST_INVALID) {
        ss->current = new_state;
    } else {
        printf("invalid state: %d.\n", new_state);
    }
    return ss->current;
}

/* ---------- Event ---------- */

const char *event_name(Event e) { // <2>
    switch (e) {
        case EV1: return "ev1";
        case EV2: return "ev2";
        case EV3: return "ev3";
        default:  return "unknown";
    }
}

/* ---------- Sample ---------- */

void sample_init(Sample *s) {
    sample_state_init(&s->state);
    s->attr_a = true;
    s->attr_b = true;
}

static bool gd1(const Sample *s) {
    return s->attr_a && s->attr_b;
}

static bool gd2(const Sample *s) {
    return !s->attr_a || s->attr_b;
}

static void act1(Event evt, int prm) {
    printf("     act1: event:%s, param: %d\n", event_name(evt), prm);
}

static void act2(Event evt, int prm) {
    printf("     act2: event:%s, param: %d\n", event_name(evt), prm);
}

static void act3(Event evt, int prm) {
    printf("     act3: event:%s, param: %d\n", event_name(evt), prm);
}

static void st0_proc(Sample *s, Event evt, int p01) {
    switch (evt) {
    case EV1:
        if (gd1(s)) {
            printf("    gd1: true\n");
            act1(evt, p01);
            sample_state_transit_to(&s->state, ST1);
        } else {
            printf("    <<< gd1: false, transition is ignored. >>>\n");
        }
        break;
    case EV3:
        act3(evt, p01);
        sample_state_transit_to(&s->state, ST2);
        break;
    default:
        break;
    }
}

static void st1_proc(Sample *s, Event evt, int p01) {
    switch (evt) {
    case EV2:
        act2(evt, p01);
        printf("    gd2: %s\n", gd2(s) ? "true" : "false");
        if (gd2(s)) {
            sample_state_transit_to(&s->state, ST2);
        } else {
            act3(evt, p01);
            sample_state_transit_to(&s->state, ST0);
        }
        break;
    case EV3:
        act3(evt, p01);
        sample_state_transit_to(&s->state, ST0);
        break;
    default:
        break;
    }
}

void sample_play(Sample *s, Event evt, int p01) {
    printf("%s ->\n", state_name(s->state.current));
    printf("  event:%s, param: %d\n", event_name(evt), p01);

    switch (s->state.current) {
        case ST0: st0_proc(s, evt, p01); break;
        case ST1: st1_proc(s, evt, p01); break;
        case ST2: /* none */ break;
        default:  break;
    }

    printf("       -> %s\n", state_name(s->state.current));
    if (s->state.current == ST2) {
        printf("finished.\n");
    }
}

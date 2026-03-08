#include <stdio.h>
#include <time.h>
#include "sample_test.h"

static void test_base(const Event *events_list[], int list_size,
                      const int *sizes, bool g0, bool g1) {
    for (int i = 0; i < list_size; i++) {
        Sample samp;
        sample_init(&samp);
        samp.guards[0] = g0;
        samp.guards[1] = g1;

        for (int j = 0; j < sizes[i]; j++) {
            int p = (int)(clock() % 1000000);
            sample_play(&samp, events_list[i][j], p);
        }
        printf("----------------\n");
    }
}

static void test01(void) {
    static const Event seq0[] = { EV1, EV2 };
    static const Event seq1[] = { EV3 };
    static const Event seq2[] = { EV1, EV3, EV3 };

    static const Event *events_list[] = { seq0, seq1, seq2 };
    static const int sizes[] = { 2, 1, 3 };

    test_base(events_list, 3, sizes, true, true);
}

static void test02(void) {
    static const Event seq0[] = { EV1, EV3 };
    static const Event seq1[] = { EV1, EV2, EV3 };

    static const Event *events_list[] = { seq0, seq1 };
    static const int sizes[] = { 2, 3 };

    test_base(events_list, 2, sizes, false, false);
}

static void test03(void) {
    static const Event seq0[] = { EV1, EV2, EV3 };
    static const Event seq1[] = { EV3 };

    static const Event *events_list[] = { seq0, seq1 };
    static const int sizes[] = { 3, 1 };

    test_base(events_list, 2, sizes, true, false);
}

void sample_test_run(void) {
    printf("================\n");
    test01();
    printf("================\n");
    test02();
    printf("================\n");
    test03();
}

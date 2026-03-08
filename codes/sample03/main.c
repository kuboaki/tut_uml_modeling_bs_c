#include <stdio.h>
#include "sample.h"
#include "sample_test.h"

int main(void) {
    /* SampleState の単体動作確認 */
    SampleState ss;
    sample_state_init(&ss);
    printf("%s\n", state_name(ss.current));

    printf("%s\n", state_name(sample_state_transit_to(&ss, ST2)));
    printf("%s\n", state_name(sample_state_transit_to(&ss, ST_INVALID)));  /* invalid */
    printf("%s\n", state_name(sample_state_transit_to(&ss, ST1)));
    printf("- - - -\n");

    /* テスト実行 */
    sample_test_run();

    return 0;
}

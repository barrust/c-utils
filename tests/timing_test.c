#include <unistd.h>         /*sleep*/
#include "../src/minunit.h"
#include "../src/timing.h"


char* res = NULL;

void test_setup(void) {}

void test_teardown(void) {
    free(res);
}


MU_TEST(test_timing_simple) {
    Timing t;

    timing_start(&t);
    printf("Begin sleeping test... about 61 seconds\n");
    sleep(61); /* sleep isn't exactly this many seconds... so get close! */
    timing_end(&t);
    mu_assert_double_between(60.75, 61.25, t.timing_double);

    /* set these to something we can test! */
    t.milliseconds = 0;
    t.microseconds = 150;
    res = format_time_diff(&t);

    mu_assert_string_eq("00:01:01:000.150", res);
    mu_assert_int_eq(0, t.hours);
    mu_assert_int_eq(1, t.minutes);
    mu_assert_int_eq(1, t.seconds);
    mu_assert_int_eq(0, t.milliseconds);
    mu_assert_int_eq(150, t.microseconds);
}

MU_TEST(test_default_hand_long_hours) {
    Timing t;
    /* set some times by hand */
    struct timeval tv;
    tv.tv_sec = 1000;
    tv.tv_usec = 101;
    t.start_time = tv;

    tv.tv_sec = 1000000;
    tv.tv_usec = 102;
    t.end_time = tv;
    calc_difference(&t);  /* force this call */

    long long i = timeval_diff(NULL, &t.end_time, &t.start_time);
    char msg[500] = {0};
    sprintf(msg, "Expected: 999000000001 but did not receive that value");
    mu_assert(999000000001 == i, msg);

    res = format_time_diff(&t);
    mu_assert_string_eq("277:30:00:000.001", res);

    mu_assert_int_eq(277, t.hours);
    mu_assert_int_eq(30, t.minutes);
    mu_assert_int_eq(0, t.seconds);
    mu_assert_int_eq(0, t.milliseconds);
    mu_assert_int_eq(1, t.microseconds);
}

MU_TEST(test_default_hand_usec_rev) {
    Timing t;
    struct timeval tv;
    tv.tv_sec = 100;
    tv.tv_usec = 78189;
    t.start_time = tv;

    tv.tv_sec = 7851;
    tv.tv_usec = 101;
    t.end_time = tv;
    calc_difference(&t);

    res = format_time_diff(&t);
    mu_assert_string_eq("02:09:10:921.912", res);

    mu_assert_int_eq(2, timing_get_hours(t));
    mu_assert_int_eq(9, timing_get_minutes(t));
    mu_assert_int_eq(10, timing_get_seconds(t));
    mu_assert_int_eq(921, timing_get_milliseconds(t));
    mu_assert_int_eq(912, timing_get_microseconds(t));
}

MU_TEST(test_default_get_functions) {
    Timing t;
    struct timeval tv;
    tv.tv_sec = 100;
    tv.tv_usec = 101;
    t.start_time = tv;

    tv.tv_sec = 7851;
    tv.tv_usec = 78189;
    t.end_time = tv;
    calc_difference(&t);  /* force this call */

    res = format_time_diff(&t);
    mu_assert_string_eq("02:09:11:078.088", res);

    mu_assert_int_eq(2, timing_get_hours(t));
    mu_assert_int_eq(9, timing_get_minutes(t));
    mu_assert_int_eq(11, timing_get_seconds(t));
    mu_assert_int_eq(78, timing_get_milliseconds(t));
    mu_assert_int_eq(88, timing_get_microseconds(t));
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_timing_simple);
    MU_RUN_TEST(test_default_hand_long_hours);
    MU_RUN_TEST(test_default_hand_usec_rev);
    MU_RUN_TEST(test_default_get_functions);
}

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}

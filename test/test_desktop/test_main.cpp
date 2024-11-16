#include <unity.h>
#include <tension.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_calculator_addition(void) {
   Tension tension;
   tension.getBrightColourIndex(0);
    TEST_ASSERT_EQUAL(28, tension.getColourIndex(0));

}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculator_addition);
    UNITY_END();

    return 0;
}
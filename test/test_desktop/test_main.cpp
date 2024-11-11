#include <Arduino.h>
#include <unity.h>
#include "main.h"

void test_calculator_addition(void) {
    TEST_ASSERT_EQUAL(32,32);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculator_addition);
    UNITY_END();

    return 0;
}
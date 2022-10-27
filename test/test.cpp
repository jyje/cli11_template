#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <gtest/gtest.h>
#include "../source/args.hpp"

TEST(simple_math_case, add) {
    EXPECT_EQ(1+2,  3);
}

TEST(simple_math_case, equal_1) {
    EXPECT_EQ(1,  1);
}

TEST(simple_math_case, equal_2) {
    EXPECT_GT(FLT_EPSILON, abs(sinf(M_PI_2)-1.0f));
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

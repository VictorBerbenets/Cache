#include "test_generating.hpp"

#define __LFU_TEST_GENERATING__
#define __PERFECT_TEST_GENERATING__

int main() {
    namespace test = Tests;

#if defined(__LFU_TEST_GENERATING__) || defined(__PERFECT_TEST_GENERATING__)
    test::generator gen;
    gen.generate(50);
#endif


}

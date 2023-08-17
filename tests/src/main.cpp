#include "test_generating.hpp"

#define __LFU_TEST_GENERATING__
#define __PERFECT_TEST_GENERATIN__

int main() {
    namespace test = Tests;

#ifdef __LFU_TEST_GENERATING__
    test::lfu_generator lfu_gen;
    gen.generate(50);
#endif


}

#include "test_generating.hpp"



int main(int argc, const char* argv[]) {
    namespace test = Tests;

#if defined(__LFU_TEST_GENERATING__) || defined(__PERFECT_TEST_GENERATING__)
    std::cout << "AAA";
    test::generator gen;
    gen.generate(50);
#endif


}

#include "test_generating.hpp"



int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect command line arguments! Expected number of tests\n";   
    }
    int64_t tests_number{};
    try {
        tests_number = std::stoi(argv[1]);
        if (tests_number < 0) {
            throw std::invalid_argument{"Got negative number for number of tests\n"};
        }
    } catch(std::invalid_argument& inv_arg) {
        std::cout << inv_arg.what() << std::endl; 
    }

#if defined(LFU_TEST_GENERATING_) || defined(PERFECT_TEST_GENERATING_)
    Tests::generator gen;
    gen.generate(tests_number);
#endif

}

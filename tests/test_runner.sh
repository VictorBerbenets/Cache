#!/bin/bash

#color atributes
red="x1b[31m"
green="x1b[32m"
usual="x1b[0m"

lfu_tests_dir="lfu_resources/"
lfu_tests="${lfu_tests_dir}tests/"
lfu_ans="${lfu_tests_dir}answers/"

lfu_cache_out="./../LFU_Cache/build/lfu_cache"

perfect_tests_dir="perfect_resources/"
perfect_tests="{perfect_tests_dir}tests/"
perfect_ans="{perfect_tests_dir}answers/"

perfect_cache_out="./../Perfect_Caching/build/perfect_cache"

function run_tests {
    echo "generating tests..."
    cd build
    make
    ./testing $tests_number
    cd ../
    echo "done"

    echo "tests number = $tests_number"
    
    touch compare_file
    if [ $is_lfu = "true" ]
    then
        echo "lfu tests:"
        for ((i = 1; i <= $tests_number; ++i))
        do  
                $lfu_cache_out < ${lfu_tests}test${i}.txt > $compare_file  
                echo -n "Test ${i}: "
                if diff -w ${lfu_ans}answ${i}.txt compare_file
                then
                    echo echo -e "${red}failed${usual}"
                else
                    echo echo -e "${green}passed${usual}"
                fi
        done
    fi
    if [ $is_perfect = "true" ]
    then
        echo "perfect tests:"
        for ((i = 1; i <= $tests_number; ++i))
        do  
                $perfect_cache_out < ${perfect_tests}test${i}.txt > $compare_file  
                echo -n "Test ${i}: "
                if diff -w ${perfect_ans}answ${i}.txt compare_file
                then
                    echo echo -e "${red}failed${usual}"
                else
                    echo echo -e "${green}passed${usual}"
                fi
        done
    fi
}

function build_caches {
    echo "Building caches..."
    cmake -S ./ -B build/
    cd build/
    make
    cd ..
    echo "done"
}

#main part
is_lfu="false"
is_perfect="false"

build_caches

if [ $# -lt 2 ] || [ $# -gt 3 ]
then
    echo "invalid number of arguments: $#. Expected 2 or 3"
#if we have two args(cache name and number of tests)
elif [ $# -eq 2 ]
then
    tests_number=$2
    if [ $1 = "lfu" ]
    then
        is_lfu="true"
    elif [ $1 = "perfect" ]
    then
        is_perfect="true"
    else
        echo "unknown command $1"
    fi
#if we have three args(cache name1(2) cache name2(1) and number of tests)
else
    tests_number="$3"
    if [ $1 = "lfu" ] && [ $2 = "perfect" ]
    then
        is_lfu="true"
        is_perfect="true"
    elif [ $1 = "perfect" ] && [ $2 = "lfu" ]
    then
        is_perfect="true"
        is_lfu="true"
    else
        echo "invalid set of cache's names : $1 $2"
    fi
fi

run_tests 


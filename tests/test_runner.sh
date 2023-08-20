#!/bin/bash

#color atributes
red="\033[1;31m"
green="\033[1;32m"
blue="\033[1;36m"
purple="\033[1;35m"
white="\033[1;37m"
usual="\033[0m"

lfu_tests_dir="lfu_resources/"
lfu_tests="${lfu_tests_dir}tests/"
lfu_ans="${lfu_tests_dir}answers/"

lfu_cache_out="./../LFU_Cache/build/lfu_cache"

perfect_tests_dir="perfect_resources/"
perfect_tests="${perfect_tests_dir}tests/"
perfect_ans="${perfect_tests_dir}answers/"

perfect_cache_out="./../Perfect_Caching/build/perfect_cache"

function run_tests {
    echo -e "${blue}generating tests...${usual}"
    cd build
    make
    ./testing $tests_number
    cd ../
    echo -e "${blue}done${usual}"
 
    touch compare_file
    if [ $is_lfu = "true" ]
    then
        echo -e "${white}lfu tests:${usual}"
        for ((i = 1; i <= $tests_number; ++i))
        do  
                $lfu_cache_out < ${lfu_tests}test${i}.txt > compare_file
                echo -n -e "${purple}Test ${i}: ${usual}"
                if diff -w ${lfu_ans}answ${i}.txt compare_file &>/dev/null
                then
                    echo -e "${green}passed${usual}" 
                else
                    echo -e "${red}failed${usual}"
                fi
                cat compare_file
        done
    fi
    if [ $is_perfect = "true" ]
    then
        echo -e "${white}perfect tests:${usual}"
        for ((i = 1; i <= $tests_number; ++i))
        do  
                $perfect_cache_out < ${perfect_tests}test${i}.txt > compare_file  
                if diff -w ${perfect_ans}answ${i}.txt compare_file &>/dev/null
                then
                    echo -e "${green}passed${usual}"
                else
                    echo -e "${red}failed${usual}"
                fi
                cat compare_file
        done
    fi
}

function build_caches {
    echo -e "${blue}Building caches...${usual}"
    cmake -S ./ -B build/
    cd build/
    make
    cd ..
    echo -e "${blue}done${usual}"
}

#main part
is_lfu="false"
is_perfect="false"

build_caches

if [ $# -lt 2 ] || [ $# -gt 3 ]
then
    echo -e "${red}invalid number of arguments:${usual} $#. Expected 2 or 3"
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
        echo -e "${red}unknown command:${usual} $1"
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
        echo -e "${red}invalid set of cache's names :${usual} $1 $2"
    fi
fi

run_tests 


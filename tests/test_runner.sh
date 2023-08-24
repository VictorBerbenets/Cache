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

lfu_cache_out="./../build/lfu"

perfect_tests_dir="perfect_resources/"
perfect_tests="${perfect_tests_dir}tests/"
perfect_ans="${perfect_tests_dir}answers/"

perfect_cache_out="./../build/perfect"

##------------------------------------------------------------------------------------------##
function run_tests { 
    generate_tests
    touch compare_file
    if [ ${is_lfu} = "true" ]
    then
        if [ -d "${lfu_tests_dir}" ]
        then
            echo -e "${white}lfu tests:${usual}"
            for ((i = 1; i <= ${tests_number}; ++i))
            do  
                    $lfu_cache_out < ${lfu_tests}/test${i}.txt > compare_file
                    echo -n -e "${purple}Test ${i}: ${usual}"
                    if diff -w ${lfu_ans}answ${i}.txt compare_file &>/dev/null
                    then
                        echo -e "${green}passed${usual}" 
                    else
                        echo -e "${red}failed${usual}"
                    fi
                    echo -en "${blue}hits:${usual} "
                    cat compare_file
            done
        else
            echo -e "${blue}can't generate tests. ${usual}${lfu_tests_dir} is empty"
        fi
    fi
    if [ ${is_perfect} = "true" ]
    then
        if [ -d "${perfect_tests_dir}" ]
        then
            echo -e "${white}perfect tests:${usual}"
            for ((i = 1; i <= ${tests_number}; ++i))
            do      
                    ${perfect_cache_out} <${perfect_tests}test${i}.txt > compare_file  
                    echo -n -e "${purple}Test ${i}: ${usual}"
                    if diff -w ${perfect_ans}answ${i}.txt compare_file &>/dev/null
                    then
                        echo -e "${green}passed${usual}"
                    else
                        echo -e "${red}failed${usual}"
                    fi
                    echo -en "${blue}hits:${usual} "
                    cat compare_file
            done
        else
            echo -e "${blue}can't generate tests. ${usual}${perfect_tests_dir} is empty"
        fi
    fi
    rm compare_file
}
##------------------------------------------------------------------------------------------##

function generate_tests {
    echo -e "${blue}generating tests...${usual}"
    cd build
    cmake --build .
    ./testing $tests_number
    cd ../
    echo -e "${blue}done${usual}"
}

##------------------------------------------------------------------------------------------##
function build_caches {
    echo -e "${blue}Building caches...${usual}"
    cmake -S ./ -B build/
    cd build/
    cmake --build .
    cd ..
    echo -e "${blue}done${usual}"
}
##------------------------------------------------------------------------------------------##

#main part
is_lfu="false"
is_perfect="false"
error="false"
tests_number=0

build_caches

if [ $# -ne 2 ]
then
    error="true"
    echo -e "${red}invalid number of arguments: expected 2, got $#"
else
    tests_number=$2
    if [ $1 = "lfu" ]
    then
        is_lfu="true"
    elif [ $1 = "perfect" ]
    then
        is_perfect="true"
    else
        error="true"
        echo -e "${red}invalid command: ${usual} $1. expected 'lfu' or 'perfect'" 
    fi
fi

if [ $error = "false" ]
then
    run_tests 
fi

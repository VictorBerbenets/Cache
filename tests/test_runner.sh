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

function run_tests {
    echo -e "${blue}generating tests...${usual}"
    cd build
    cmake --build .
    ./testing $tests_number
    cd ../
    echo -e "${blue}done${usual}"
 
    touch compare_file
    #if lfu tests dir is empty
    if ! [ `ls ${lfu_tests} | wc -l` -eq 0 ]
    then
        echo -e "${white}lfu tests:${usual}"
        for file in ${lfu_tests}*
        do  
                $lfu_cache_out < file > compare_file
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
    fi
    #if perfect tests dir is empty
    if ! [ `ls ${perfect_tests} | wc -l` -eq 0 ]

    then
        echo -e "${white}perfect tests:${usual}"
        for file in ${perfect_tests}*
        do      
                echo "file = ${file}"
                $perfect_cache_out < file > compare_file  
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
error="false"
tests_number=0

build_caches

if [ $# -ne 1 ]
then
    error="true"
    echo -e "${red}invalid number of arguments:${usual} $#. Expected 1, got ${$#}"
else
    tests_number=$1
fi

if [ $error = "false" ]
then
    run_tests 
fi

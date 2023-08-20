#!/bin/bash

#color atributes
red="x1b[31m"
green="x1b[32m"
perfect_tests="{perfect_tests_dir}tests/"
usual="x1b[0m"

lfu_tests_dir="lfu_resources/"
lfu_tests="${lfu_tests_dir}tests/"
lfu_ans="${lfu_tests_dir}answers/"

lfu_cache="./../LFU_Cache/build/lfu_cache"

perfect_tests_dir="perfect_resources/"
perfect_tests="{perfect_tests_dir}tests/"
perfect_ans="{perfect_tests_dir}answers/"

perfect_cache="./../Perfect_Caching/build/perfect_cache"


function run_tests {


}

is_lfu="false"
is_perfect="false"

if [$# -lt 2] || [$# -gt 3]
then
    echo "invalid number of arguments: $#. Expected 2 or 3"
#if we have two args(cache name and number of tests)
elif [$# -eq 2]
then
    tests_number=$2
    if [$1 = "lfu"]
    then
        is_lfu="true"

        echo "Building lfu tests..."
        cd

    elif [$1 = "perfect"]
    then
        is_perfect="true"
    else
        echo "unknown command $1"
    fi
else
    tests_number="$3"
    if [$1 = "lfu"] && [$2 = "perfect"]
    then
        is_lfu="true"
        is_perfect="true"
    elif [$1 = "perfect"] && [$2 = "lfu"]
    then
        is_perfect="true"
        is_lfu="true"
    else
        echo "invalid set of cache's names : $1 $2"
fi

run_tests 




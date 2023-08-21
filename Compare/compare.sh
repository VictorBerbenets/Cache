#!/bin/bash
#
#color atributes
red="\033[1;31m"
blue="\033[1;36m"
usual="\033[0m"

comp_files="../tests/perfect_resources/tests"
del_files="../tests/perfect_resources/answers"
tests_out="./../tests/build/testing"
comp_out="./build/compare"

tests_type="perfect"

#---------------------------------------------#
function generate_comp_files {
    ${tests_out} ${compares_count}
    echo "tests dir: "
    ls ${comp_files}
    for file in ${comp_files}
    do
        ${comp_out} $file
    done
}
#---------------------------------------------#

#---------------------------------------------#
function clear_compareTXT {
    echo -n > comparing.txt    
}
#---------------------------------------------#

###main()###
error="false"
compares_count=0

if [ $# -ne 1 ]
then
    error="true"
    echo -e "${red}error:${usual} invalid set of arguments"
elif ! [ `ls ${comp_files} | wc -l` -eq 0 ] 
    then
        rm ${del_files}/*answ*
        rm ${comp_files}/*test*
        compares_count=$1
fi

if [ ${error} = "false" ]
then
    echo "generating 'perfect tests'"
    ${tests_out} ${compares_count}
    clear_compareTXT
    generate_comp_files
fi

#---------------------------------------------#


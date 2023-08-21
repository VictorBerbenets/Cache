#!/bin/bash
#
#color atributes
red="\033[1;31m"
blue="\033[1;36m"
usual="\033[0m"


comp_files="../tests/perfect_resources/tests/"
del_files="../tests/perfect_resources/answers"
tests_out_dirr="../tests/build"
tests_out_file="./testing"
comp_out="./build/compare"


#---------------------------------------------#
function generate_comp_files {
    cd ${tests_out_dirr}
    echo -e "${blue}generating compare files...${usual}"
    ${tests_out_file} ${compares_count}
    cd ../../Compare
    for file in ${comp_files}*
    do
        ${comp_out} $file
    done
    echo -e "${blue}done${usual}"
}
#---------------------------------------------#


#---------------------------------------------#
function clear_compareTXT {
    echo -n > comparing.txt    
}
#---------------------------------------------#

function remove_old_data {
    rm ${del_files}/*answ*
    rm ${comp_files}/*test*
}

###main()###
error="false"
compares_count=0

if [ $# -ne 1 ]
then
    error="true"
    echo -e "${red}error:${usual} invalid set of arguments"
elif ! [ `ls ${comp_files} | wc -l` -eq 0 ] 
then
    remove_old_data
fi

compares_count=$1
if [ ${error} = "false" ]
then
    clear_compareTXT
    generate_comp_files
fi
#---------------------------------------------#


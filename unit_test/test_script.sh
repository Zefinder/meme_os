#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
WHITE='\033[1;37m'
NO_COLOR='\033[0m'

test_utils_c="test_utils.c"
script_file="test_script.sh"
test_file="test"
untestable_functions="untestable_functions.c"
filenames=($(ls | grep .c | cut -d' ' -f1))

CC="gcc"
OPTIONS="-fms-extensions -rdynamic -finstrument-functions -Wno-builtin-declaration-mismatch"
INCLUDES="-I../kernel/include -I."
objects=$@

test_number=0
has_error=0

compile_and_test () {
    local filename=$1

    # Compile
    echo -e ${WHITE}Compiling file $filename...${NO_COLOR}
    ${CC} ${OPTIONS} ${INCLUDES} -o $test_file $objects $test_utils_c $untestable_functions $filename -ldl
    
    if [ $? -ne 0 ]; then
        echo -e ${RED}\[ERROR\] Impossible to compile $filename, check gcc error for more information...${NO_COLOR}
        return 1
    fi

    # Execute
    echo -e ${WHITE}Executing $filename...${NO_COLOR}
    ./$test_file
    if [ $? -ne 0 ]; then
        echo -e ${RED}\[ERROR\] Error while executing $filename, either test failed or there is another error...${NO_COLOR}
        return 1
    fi

    echo -e ${GREEN}\[SUCCEES\] Test succeeded for $filename!${NO_COLOR}
}

for filename in "${filenames[@]}"; do
    if [ "$filename" != "$test_utils_c" ] && [ "$filename" != "$script_file" ] && [ "$filename" != "$untestable_functions" ]; then
        # One new test
        test_number=$(($test_number + 1))

        # Testing file
        compile_and_test $filename

        # Added error to count
        has_error=$(($has_error + $?))
    fi
done

# Deleting binary test file
if [ -f "$test_file" ]; then
    rm $test_file
fi

# Final echos
echo Executed tests: $test_number
echo Test failed: $has_error
if [ "$has_error" -ne 0 ]; then
    echo -e ${RED}\[ERROR\] Tests failed, read errors above!${NO_COLOR}
    exit 1
else 
    echo -e ${GREEN}\[SUCCESS\] All tests succeeded!${NO_COLOR}
    exit 0
fi

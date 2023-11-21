#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
WHITE='\033[1;37m'
NO_COLOR='\033[0m'

test_utils="test_utils.c"
filenames=($(ls | grep .c | cut -d' ' -f1))
i=0
has_error=0

compile_and_test () {
    local filename=$1

    # Compile
    echo -e ${WHITE}Compiling file $filename...${NO_COLOR}

    if [ $? -ne 0 ]; then
        echo -e ${RED}\[ERROR\] Impossible to compile $filename, check gcc error for more information...${NO_COLOR}
        return 1
    fi

    # Execute
    echo -e ${WHITE}Executing $filename...${NO_COLOR}

    if [ $? -ne 0 ]; then
        echo -e ${RED}\[ERROR\] Error while executing $filename, either test failed either there is another error...${NO_COLOR}
        return 1
    fi

    echo -e ${GREEN}Test succeeded for $filename!${NO_COLOR}
}

for filename in "${filenames[@]}"; do
    i=$(($i+1))
    if [ "$filename" = "$test_utils" ]; then
        compile_and_test $filename
        has_error=$(($? + $has_error))
    fi
done

echo Test failed: $has_error
if [ "$has_error" -ne 0 ]; then
    echo -e ${RED}\[ERROR\] Tests failed, read errors above!${NO_COLOR}
    exit 1
else 
    echo -e ${GREEN}All tests succeeded!${NO_COLOR}
    exit 0
fi
# echo Loop steps: $i

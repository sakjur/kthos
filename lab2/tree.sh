#!/bin/bash

function printout {
    # I used for, but changed to while since `for` breaked on space
    # while `while` breaks on new line, which allows for spaces in
    # filenames

    # $1 is the current folder
    # $2 is the depth of the current folder
    # Bash doesn't seem to like local variables, so not giving them
    # human-friendly names

    ls -p "$1" | while read FILE
    do
        for UNIT in $2
        do
            echo -n "-";
        done

        echo $FILE;
        if [ ${FILE: -1} = "/" ]; then # Last character of variable
            NEWFOLDER=$1"/"$FILE;
            printout "$NEWFOLDER" "$2 1";
        fi
    done
}

# On no argument, use current directory. Otherwise use first argument as dir
if [ $1 = "" ]; then
    FOLDER=`pwd`;
else
    FOLDER=$1;
fi

printout $FOLDER "1";


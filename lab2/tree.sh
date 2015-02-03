#!/bin/bash

if [ $1 = "" ]; then
    FOLDER=`pwd`;
else
    FOLDER=$1;
fi

echo $FOLDER;

function printout {
    # I used for, but changed to while since `for` breaked on space
    # while `while` breaks on new line, which allows for spaces in
    # filenames

    ls -p "$1" | while read FILE
    do
        for UNIT in $2
        do
            echo -n "-";
        done

        echo $FILE;
        if [ ${FILE: -1} = "/" ]; then
            NEWFOLDER=$1"/"$FILE;
            printout "$NEWFOLDER" "$2 1";
        fi
    done
}

printout $FOLDER "1";


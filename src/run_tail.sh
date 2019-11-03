#!/bin/bash


if [ "$1" == "" ]; then
    echo "Usage: ./run_tail.sh PATH_TO_BE_SCANNED"
    exit 1
fi

rm Duplicates.txt
touch Duplicates.txt
gnome-terminal --title "DuplicatesTail" --window --maximize -e "tail -f Duplicates.txt" && ./main.out "$1"


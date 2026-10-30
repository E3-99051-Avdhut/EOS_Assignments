#!/bin/bash

echo "Executable files in the current directory:"

for file in *
do
    if [ -f "$file" ] && [ -x "$file" ]
    then
        echo "$file"
    fi
done


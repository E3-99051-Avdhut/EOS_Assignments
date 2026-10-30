#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 <filename>"
    exit 1
fi

if [ -f "$1" ]
then
    echo "Last modification time of $1:"
    stat -c "%y" "$1"
else
    echo "File does not exist."
fi


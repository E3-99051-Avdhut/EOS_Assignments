#!/bin/bash

echo "Enter the first filename:"
read file1

echo "Enter the second filename:"
read file2

if [ -f "$file1" ]
then
    tr 'a-zA-Z' 'A-Za-z' < "$file1" >> "$file2"
    echo "Contents appended successfully."
else
    echo "First file does not exist."
fi


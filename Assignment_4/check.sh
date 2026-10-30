#!/bin/bash

echo "Enter file or directory name : "
read name

if [ -f "$name" ]
then
	echo "It is a file."
	echo "File size : "
	du -h "$name"

elif [ -d $name ]
then
	echo "It is a directory."
	echo "Directory contents : "
	ls "$name"

else
	echo "File or directory does not exist."

fi




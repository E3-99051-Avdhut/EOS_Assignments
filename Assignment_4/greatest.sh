#!/bin/bash

echo -n "Enter the three numbers : "
read a b c

if [ "$a" -ge "$b" ] && [ "$a" -ge "$c" ]
then
	echo "$a is the greatest number."

elif [ "$b" -ge "$c" ] && [ "$b" -ge "$c" ]
then
	echo "$b is the greatest number."

else
	echo "$c is the greatest number."

fi


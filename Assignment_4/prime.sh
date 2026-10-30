#!/bin/bash

echo -n "Enter a Number : "
read n

if [ "$n" -le 1 ]
then
	echo "$n is not a prime Number."
	exit
fi

i=2

while [ $i -lt $n ]
do
	if [ $((n % i)) -eq 0 ]
	then
		echo "$n is not a prime number."
		exit
	fi

	i=$((i + 1))
done

echo "$n is a prime number."

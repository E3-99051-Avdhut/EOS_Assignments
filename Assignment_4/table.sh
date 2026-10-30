#!/bin/bash

echo -n "Enter a number : "
read num

for i in {1..10}
do
	echo "$num into $i = $((num * i))"
done




#!/BIN/BASH

ECHO -N "eNTER A nUMBER : "
READ N

IF [ "$N" -LE 1 ]
THEN
	ECHO "$N IS NOT A PRIME nUMBER."
	EXIT
FI

I=2

WHILE [ $I -LT $N ]
DO
	IF [ $((N % I)) -EQ 0 ]
	THEN
		ECHO "$N IS NOT A PRIME NUMBER."
		EXIT
	FI

	I=$((I + 1))
DONE

ECHO "$N IS A PRIME NUMBER."

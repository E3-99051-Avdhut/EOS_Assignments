#!/bin/bash

echo "Enter basic salary:"
read basic

da=$(echo "scale=2; $basic * 0.40" | bc)
hra=$(echo "scale=2; $basic * 0.20" | bc)

gross=$(echo "scale=2; $basic + $da + $hra" | bc)

echo "Basic Salary = $basic"
echo "DA = $da"
echo "HRA = $hra"
echo "Gross Salary = $gross"


#!/bin/bash

while true
do
	echo "-------MENU-------"
	echo "1. Date"
	echo "2. Cal"
	echo "3. Ls"
	echo "4. Pwd"
	echo "5. Exit"

	echo "Enter your choice : "
	read choice

	case $choice in
		1)
		  date
		  ;;

		2)
		  cal
		  ;;

		3)
		  ls
		  ;;

		4)
		  pwd
		  ;;

		5)
		  echo "Existing the program..."
		  exit
		  ;;

		*)
		  echo "Invalid choice! Please try again."
		  ;;
	esac

	echo ""
done	






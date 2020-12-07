#!/bin/bash
# Authors: Vojtěch Vlach (xvlach22)
# Run tests in tests/generator/generator-test.c, save the output and interpret it as .ifjcode program
# For correct response run with:
# %PROJECTDIR%/src$ ./../tests/generator/generator-test.sh <option>

readonly TESTBUILD="./../build/generator-test"
readonly TESTOUTFILE="../build/generator-test.ifjcode"
readonly IFJCODEINTERPRET="./../tests/generator/ic20int"
readonly FALSE=0
readonly TRUE=1

printall=$FALSE
valgrind=$FALSE

makeAndRun(){
	make generator-test

	if [ $valgrind -eq $TRUE ]; then
		valgrind $TESTBUILD > $TESTOUTFILE
	else
		$TESTBUILD > $TESTOUTFILE

		if [ $printall -eq $TRUE ]; then 
			echo "-------ouptut.ifjcode:"
			cat ../build/generator-test.ifjcode
		fi

		echo "------------ifjcode20:"
		$IFJCODEINTERPRET $TESTOUTFILE
	fi
}

#--------------------- START ----------------------#

case $1 in 
	-r)
		makeAndRun
		exit $CODE
		;;
	--run)
		makeAndRun
		exit $CODE
		;;
	-a)
		printall=$TRUE
		makeAndRun
		exit $CODE
		;;
	--all)
		printall=$TRUE
		makeAndRun
		exit $CODE
		;;
	-v)
		valgrind=$TRUE
		makeAndRun
		exit $CODE
		;;
	--valgrind)
		valgrind=$TRUE
		makeAndRun
		exit $CODE
		;;
	*)
		echo "Run tests in tests/generator/generator-test.c, save the output and interpret it as .ifjcode program"
		echo "For correct response run with:"
		echo "	%PROJECTDIR%/src$ ./../tests/generator/generator-test.sh <option>"
		echo "List of options:"
		echo "	-r or --run Make and run with ic20int"
		echo "	-a or --all for make and run with ic20int and print all to stdout"
		echo "	-v or --valgrind. Make and run with valgrind. (without ic20int)"
		exit $CODE
		;;
esac


exit 

# This file compares two outputs from inter_code-test.c
# and prints th result
# For correct response run with:
# %PROJECTDIR%/src$ ./../tests/inter_code/inter_code-test.sh
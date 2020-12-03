#!/bin/bash
# Authors: Vojtěch Vlach (xvlach22)
# This file compares two outputs from inter_code-test.c
# and prints th result
# For correct response run with:
# %PROJECTDIR%/src$ ./../tests/inter_code/inter_code-test.sh

PROGRAMNAME="inter_code"
PROGRAM="./../build/${PROGRAMNAME}-test"
SOURCEFILE="./../tests/${PROGRAMNAME}/${PROGRAMNAME}-test-model.out"
TESTFILE="./../tests/${PROGRAMNAME}/${PROGRAMNAME}-test-my.out"
MAKEFILEDIR="./../../src"

testFile(){
	if [ ! -f ${PROGRAM} ]; then
		echo "Missing executable, running make."
		#make -i ${MAKEFILEDIR}
		make
	fi
	if [ -f ${SOURCEFILE} ]; then
		make
		./${PROGRAM} > ${TESTFILE}
		diff -s $SOURCEFILE $TESTFILE
	else
		echo "Source file Missing"
	fi
}

upgradeFile(){
	if [ ! -f ${PROGRAM} ]; then
		echo "Missing executable, running make."
		#make -i ${MAKEFILEDIR}
		make
	fi
	./${PROGRAM} > ${SOURCEFILE}
	echo "Upgrading successfull"
}

makeAndRun(){
	make &&	./${PROGRAM}
}

makeAndValgrindRun(){
	make &&	valgrind ./${PROGRAM}
}

#--------------------- START ---------------------#

case $1 in 
	--test)
		testFile
		exit $CODE
		;;
	-t)
		testFile
		exit $CODE
		;;
	--upgrade)
		upgradeFile
		exit $CODE
		;;
	-u)
		upgradeFile
		exit $CODE
		;;
	--run)
		makeAndRun
		exit $CODE
		;;
	-r)
		makeAndRun
		exit $CODE
		;;
	--valgrind)
		makeAndValgrindRun
		exit $CODE
		;;
	-v)
		makeAndValgrindRun
		exit $CODE
		;;
	*)
		echo "-t or --test for testing file"
		echo "-u or --upgrade for upgrading test file"
		echo "-r or --run for make and run"
		echo "-v or --valgrind for make and run with valgrind"
		exit $CODE
		;;
esac
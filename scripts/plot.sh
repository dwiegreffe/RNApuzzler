#!/bin/bash
#########################################################################
#                                                                       #
#  Script: PLOT                                                         #
#  Author: Daniel Alexander                                             #
#                                                                       #
#  What it does:                                                        #
#    generates PS files for all FOLD files in the current folder        #
#                                                                       #
#  The details:                                                         #
#  - initially deletes all PS files in the current folder               #
#  - calls RNAplot for all FOLD files in the current folder             #
#                                                                       #
#########################################################################

file=$1
printf "[FILE] %s" $file
cat $file | RNAplot -t3 > $file.log
results=$(grep "\[RESULT FINAL\]" $file.log)
for w in $results
do
	case "$w" in
	("[RESULT") printf "\n%s" $w;;
	("SUCCESS") printf " \x1b[32m%s\x1b[0m" $w;; # print text in green
	("FAIL")    printf " \x1b[31m%s\x1b[0m   " $w;; # print text in red
	(*)         printf " %s" $w;;
	esac
done
printf "\n"

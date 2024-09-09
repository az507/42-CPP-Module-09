#!/usr/bin/env zsh

typeset -i max_iterations

if [ $# -eq 0 ]; then
	max_iterations="20"
else
	max_iterations=$1
fi

if [ $# -gt 1 ]; then
	upper_limit=$2
else
	upper_limit=15
fi

for ((i = 0; i < $max_iterations; i++));
do
	valgrind ./PmergeMe `shuf -i 1-$upper_limit | tr "\n" " "`
done

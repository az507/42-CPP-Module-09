#!/usr/bin/env zsh

set -euo pipefail

if [ $# -ne 2 ]; then
	echo "Usage: ./test.sh [max_iterations] [upper_limit]"
	return 1
fi

max_iterations=$1
upper_limit=$2
min_var=1
max_var=$upper_limit

for ((i = 0; i < $max_iterations; i++));
do
	x=$(awk -v max=$max_var -v min=$min_var -v seed="$(od -An -N4 -tu4 /dev/urandom)" 'BEGIN{srand(seed+0); printf("%d", int(min+rand()*(max-min+1)))}')
	./PmergeMe `shuf -i 1-$x | tr "\n" " "`
done

#!/bin/bash

compile_sum=0
execution_sum=0

for i in {1..1000}
do
    echo "run $i"
    readarray -t array <<< "$(./brlc test.brl | awk '{print $3}')"
    compile_sum=$(echo "$compile_sum + ${array[0]}" | bc)
    execution_sum=$(echo "$execution_sum + ${array[1]}" | bc)
    echo -n "${array[0]}," >> comp_sum.dat
    echo -n "${array[1]}," >> exec_sum.dat
done

compile_average=$(echo "scale=8; $compile_sum / 1000" | bc)
execution_average=$(echo "scale=8; $execution_sum / 1000" | bc)
echo "Average compile time: $compile_average"
echo "Average execution time: $execution_average"

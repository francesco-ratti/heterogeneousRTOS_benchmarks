#!/bin/bash

# Iterate over each CSV file in the folder
for csv_file in *.csv; do
    # Execute the grep command and save the output to a new file
    grep -v '^[[:space:]]*$' "$csv_file" | tee "trim_${csv_file}"
done

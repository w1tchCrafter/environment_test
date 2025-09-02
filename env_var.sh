#!/usr/bin/env bash

# simple shell script to format env string array used in stack.c

# Loop over all environment variables
while IFS="=" read -r -d "" n v; do
    if [[ "$n" != "_" ]]; then
        printf \"$n\",
    fi    
done < <(env -0)

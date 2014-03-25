#!/bin/sh

# Convert PSR to C
for PSR in `ls -1 *psr`; do
    perl psr2c.pl $PSR
done

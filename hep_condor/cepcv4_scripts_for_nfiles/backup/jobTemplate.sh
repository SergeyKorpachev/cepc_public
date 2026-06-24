#!/bin/bash

echo "START OUTPUT_FILENAME: $(date +"%e %b %Y, %a %r")"

cd INITIAL_DIR
source INITIAL_DIR/setup.sh

### Run
INITIAL_DIR/run.sh INITIAL_DIR/cepcv4_OUTPUT_FILENAME.py

echo "STOP OUTPUT_FILENAME: $(date +"%e %b %Y, %a %r")"

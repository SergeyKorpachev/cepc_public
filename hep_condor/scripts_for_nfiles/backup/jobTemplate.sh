#!/bin/bash

echo "START OUTPUT_FILENAME: $(date +"%e %b %Y, %a %r")"

cd INITIAL_DIR
source INITIAL_DIR/setup.sh

### Sim
INITIAL_DIR/run.sh INITIAL_DIR/sim_OUTPUT_FILENAME.py

### Tracking
INITIAL_DIR/run.sh INITIAL_DIR/tracking_OUTPUT_FILENAME.py

### CaloDigi
INITIAL_DIR/run.sh INITIAL_DIR/calodigi_OUTPUT_FILENAME.py

### Rec
INITIAL_DIR/run.sh INITIAL_DIR/rec_OUTPUT_FILENAME.py

echo "STOP OUTPUT_FILENAME: $(date +"%e %b %Y, %a %r")"

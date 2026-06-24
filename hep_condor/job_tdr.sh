#!/bin/bash

echo "START: $(date +"%e %b %Y, %a %r")"

cd /cefs/higgs/korpachev/aaa/CEPCSW
source /cefs/higgs/korpachev/aaa/CEPCSW/setup.sh
source script_tdr_o1_v01_1000evt.sh

echo "STOP: $(date +"%e %b %Y, %a %r")"

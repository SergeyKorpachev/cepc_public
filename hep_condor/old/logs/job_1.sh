#!/bin/bash

echo "START: $(date +"%e %b %Y, %a %r")"

export PATH=/cvmfs/container.ihep.ac.cn/bin:$PATH
hep_container shell CentOS7

cd /cefs/higgs/korpachev/CEPCSW-master
source /cefs/higgs/korpachev/CEPCSW-master/setup.sh
source script_cepcv4_1000evt.sh

cd ..

echo "STOP: $(date +"%e %b %Y, %a %r")"

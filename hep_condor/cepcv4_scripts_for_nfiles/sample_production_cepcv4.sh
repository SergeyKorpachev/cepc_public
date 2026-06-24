#!/bin/bash

###****************************************************************
###  Script for TDR SAMPLE PRODUCTION:
###****************************************************************
###  Input parameter:
###  1: Number of files
###  2: Number of events
###****************************************************************

###************************************
### Check arguments:
###************************************

if [ $# -ne 2 ]; then
    echo "Args: Number of files and number of events"
    return 1
fi

###************************************
### Assign variables:
###************************************

initial_dir=/cefs/higgs/korpachev/aaa/CEPCSW
#output_dir=initial_dir

number_files=$1
number_events=$2

type_particle="K_L0"
energy_particle=20

###************************************
### Output files:
###************************************

cd ${initial_dir}
output_filename=0

#############################################
for i in $( seq 1 $number_files )
do
    output_filename=${type_particle}_${energy_particle}gev_${number_events}evt_seed${i}
    cp ${initial_dir}/backup/{cepcv4Template.py,jobTemplate.sh} ${initial_dir}/.

    sed "s@SEED_NUMBER@${i}@g" cepcv4Template.py > c1.tmp
    sed "s@TYPE_PARTICLE@${type_particle}@g" c1.tmp > c2.tmp
    sed "s@ENERGY@${energy_particle}@g" c2.tmp > c3.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" c3.tmp > c4.tmp
    sed "s@EVENTS_NUMBER@${number_events}@g" c4.tmp > c5.tmp
    cp c5.tmp cepcv4_${output_filename}.py
    rm cepcv4Template.py c*.tmp

    sed "s@INITIAL_DIR@${initial_dir}@g" jobTemplate.sh > j1.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" j1.tmp > j2.tmp
    cp j2.tmp job_${output_filename}.sh
    rm jobTemplate.sh j*.tmp
    chmod a+x job_${output_filename}.sh

    /afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin/hep_sub -np 4 -mem 6000 -wt long job_${output_filename}.sh

    echo "file ${i} done."
done

echo "Have a nice day :)"
#############################################


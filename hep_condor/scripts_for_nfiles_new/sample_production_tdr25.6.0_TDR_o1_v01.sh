#!/bin/bash

###****************************************************************
###  Script for TDR SAMPLE PRODUCTION:
###****************************************************************
###  Input parameter:
###  1: Particle type
###  2: Particle energy
###  3: Number of files
###  4: Number of events
###****************************************************************

###************************************
### Check arguments:
###************************************

if [ $# -ne 4 ]; then
    echo "Args: Particle type, particle energy, number of files, number of events"
    return 1
fi

###************************************
### Assign variables:
###************************************

initial_dir=/cefs/higgs/korpachev/samples/CEPCSW-tdr25.6.0
#output_dir=initial_dir

type_particle=$1
energy_particle=$2
number_files=$3
number_events=$4

###************************************
### Output files:
###************************************

cd ${initial_dir}
output_filename=0

#############################################
for i in $( seq 1 $number_files )
do
    output_filename=${type_particle}_${energy_particle}gev_tdr25.6.0_TDR_o1_v01_${number_events}evt_seed${i}
    cp ${initial_dir}/backup/{simTemplate.py,trackingTemplate.py,calodigiTemplate.py,recTemplate.py,jobTemplate.sh} ${initial_dir}/.

    sed "s@SEED_NUMBER@${i}@g" simTemplate.py > s1.tmp
    sed "s@TYPE_PARTICLE@${type_particle}@g" s1.tmp > s2.tmp
    sed "s@ENERGY@${energy_particle}@g" s2.tmp > s3.tmp
    sed "s@EVENTS_NUMBER@${number_events}@g" s3.tmp > s4.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" s4.tmp > s5.tmp
    cp s5.tmp sim_${output_filename}.py
    rm simTemplate.py s*.tmp

    sed "s@SEED_NUMBER@${i}@g" trackingTemplate.py > t1.tmp
    sed "s@EVENTS_NUMBER@${number_events}@g" t1.tmp > t2.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" t2.tmp > t3.tmp
    cp t3.tmp tracking_${output_filename}.py
    rm trackingTemplate.py t*.tmp

    sed "s@SEED_NUMBER@${i}@g" calodigiTemplate.py > c1.tmp
    sed "s@EVENTS_NUMBER@${number_events}@g" c1.tmp > c2.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" c2.tmp > c3.tmp
    cp c3.tmp calodigi_${output_filename}.py
    rm calodigiTemplate.py c*.tmp

    sed "s@EVENTS_NUMBER@${number_events}@g" recTemplate.py > r1.tmp
    sed "s@OUTPUT_FILENAME@${output_filename}@g" r1.tmp > r2.tmp
    cp r2.tmp rec_${output_filename}.py
    rm recTemplate.py r*.tmp

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


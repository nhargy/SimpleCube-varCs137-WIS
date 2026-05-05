#!/bin/bash

START_INDEX=$1
END_INDEX=$2
BASENAME=$3

for ((i = $START_INDEX; i <= $END_INDEX; i++)); do
    JOBID=$(printf "%04d" $i)
    OUTFILENAME="${BASENAME}_${JOBID}.root"
    PROCFILENAME="${BASENAME}_${JOBID}.parquet"
    echo "Processing ${OUTFILENAME} into ${PROCFILENAME}"

    PROCSCRIPT_PATH="/srv01/xenon/nadavh/SimpleCube/analysis/post_processing.py"
    ROOTFILE_PATH="/storage/xenon/nadavh/SimpleCube/raw/${OUTFILENAME}"
    PROCFILE_PATH="/storage/xenon/nadavh/SimpleCube/proc/${PROCFILENAME}"

    python $PROCSCRIPT_PATH --rootfile $ROOTFILE_PATH --outfile $PROCFILE_PATH 
    
done


#!/bin/bash
#PBS -N SimpleCube_job
#PBS -q N
#PBS -m n
#PBS -l walltime=10:00:00
#PBS -l select=1:ncpus=1:mem=2500mb
#PBS -l io=1
#PBS -j oe
#PBS -o /storage/xenon/{{USER}}/SimpleCube/{{DIRNAME}}/log/{{LOGFILENAME}}

# Load environment
echo "Activating LCG environment..."
source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc12-opt/setup.sh

echo " "
echo "=====> Running GEANT4 SimpleCube Sim! <====="    
cd /srv01/xenon/{{USER}}/SimpleCube/build
./SimpleCube --macro {{MACROFILE}} --primaries {{PRIMARIES}} --outfile /storage/xenon/{{USER}}/SimpleCube/{{DIRNAME}}/raw/{{OUTFILENAME}} --seed {{SEED}}

echo "=====> Processing to .parquet format"    
python ../analysis/post_processing.py --rootfile /storage/xenon/{{USER}}/SimpleCube/{{DIRNAME}}/raw/{{OUTFILENAME}} --outfile /storage/xenon/{{USER}}/SimpleCube/{{DIRNAME}}/proc/{{PROCFILENAME}}
#!/bin/bash

###########################################
# -----------  GLOBAL DEFAULTS -----------
###########################################

TEMPLATE="run_SimpleCube_single.sh"
INFOTEMPLATE="infofile_template.txt"

###########################################
# ---------------  USAGE -----------------
###########################################
usage() {
    echo "  $0 <num_jobs> <primaries_per_job> <macro_file>"
    exit 1
}

###########################################
# -----------  PARSE ARGUMENTS -----------
###########################################
if [ $# -lt 3 ]; then usage; fi
NUM_JOBS=$1
PRIMARIES=$2
MACROFILE=$3

###########################################
# ------  GENERATE SERIAL NUMBER ----------
###########################################
SERIAL="$(tr -dc 'A-Z' </dev/urandom | head -c3)$(tr -dc '0-9' </dev/urandom | head -c3)_$(date +%d%m%y)"

###########################################
# ----------  BUILD INFO FILE ----------- #
###########################################
build_infofile() {
    local infopath="$1"
    local dirname="$2"
    local primaries="$3"
    local macro="$4"
    local njobs="$5"
    local datetime="$6"

    sed \
        -e "s|{{PRIMARIES}}|$primaries|g" \
        -e "s|{{MACROFILE}}|$macro|g" \
        -e "s|{{USER}}|$USER|g" \
        -e "s|{{DIRNAME}}|$dirname|g" \
        -e "s|{{NJOBS}}|$njobs|g" \
        -e "s|{{DATETIME}}|$datetime|g" \
        "$INFOTEMPLATE" > "$infopath"

    line=16

    macrofilepath="../macros/${macro}"
    tmp=$(mktemp)
    awk -v n="$line" -v src="$macrofilepath" '
    NR==n {
        while ((getline l < src) > 0) print l
        close(src)
    }
    { print }
    ' "$infopath" > "$tmp" && mv "$tmp" "$infopath"
}

###########################################
# ------------ BUILD JOBSCRIPT -----------
###########################################
build_jobscript() {
    local jobscript="$1"source /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-el9-gcc12-opt/setup.sh
    local primaries="$2"
    local outfilename="$3"
    local macro="$4"
    local procfilename="$5"
    local seed="$6"
    local dirname="$7"
    local logfilename="$8"

    sed \
        -e "s|{{PRIMARIES}}|$primaries|g" \
        -e "s|{{OUTFILENAME}}|$outfilename|g" \
        -e "s|{{MACROFILE}}|$macro|g" \
        -e "s|{{PROCFILENAME}}|$procfilename|g" \
        -e "s|{{USER}}|$USER|g" \
        -e "s|{{SEED}}|$seed|g" \
        -e "s|{{DIRNAME}}|$dirname|g" \
        -e "s|{{LOGFILENAME}}|$logfilename|g" \
        "$TEMPLATE" > "$jobscript"

    chmod +x "$jobscript"
}

###########################################
# --------------- SUBMIT JOB -------------
###########################################
submit_job() {
    local jobscript="$1"
    local basename="$2"

    qsub "$jobscript"
}

###########################################
# ---------------- MAIN LOOP -------------
###########################################
TMPDIR=$(mktemp -d)
END_INDEX=$((1 + NUM_JOBS - 1))
MACRO_BASENAME=$(basename "$MACROFILE")
MACRO_TAG="${MACRO_BASENAME%.*}"
MACRO_TAG="${MACRO_TAG#run_}"
DIRNAME="${MACRO_TAG}-${SERIAL}"
INFOPATH="/storage/xenon/${USER}/SimpleCube/${DIRNAME}/info.txt"

# Make data storage directories
mkdir -p "/storage/xenon/$USER/SimpleCube/$DIRNAME/raw"
mkdir -p "/storage/xenon/$USER/SimpleCube/$DIRNAME/proc"
mkdir -p "/storage/xenon/$USER/SimpleCube/$DIRNAME/log"

DATETIME="$(date +"%d-%m-%y %H:%M:%S")"
build_infofile "$INFOPATH" "$DIRNAME" "$PRIMARIES" "$MACROFILE" "$NUM_JOBS" "$DATETIME"

for ((i = 1; i <= END_INDEX; i++)); do
    JOBID=$(printf "%04d" $i)
    BASENAME="${MACRO_TAG}_${JOBID}"
    OUTFILENAME="${BASENAME}.root"
    LOGFILENAME="${BASENAME}.log"
    PROCFILENAME="${BASENAME}.parquet"
    echo $OUTFILENAME
    echo $PROCFILENAME

    SEED=$(( ( $(od -An -N4 -tu4 /dev/urandom | tr -d ' ') % 2147483647 ) + 1 ))
    echo "Seed: ${SEED}"

    JOBSCRIPT="$TMPDIR/hermeticsub_${JOBID}.sh"
    build_jobscript "$JOBSCRIPT" "$PRIMARIES" "$OUTFILENAME" "$MACROFILE" "$PROCFILENAME" "$SEED" "$DIRNAME" "$LOGFILENAME"
    submit_job "$JOBSCRIPT" "$BASENAME"
done

rm -r "$TMPDIR"
echo "All jobs processed."

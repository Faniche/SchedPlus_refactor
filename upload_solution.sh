#!/bin/bash

solution_dir="cmake-build-debug/SolutionReport"

# $1: topology
# $2: streamNum
# $3: strategy
# $4: runId
# $5: solutionId
upload_xmls=$(ls $solution_dir/"$1"/"$2"/"$3"/"$4"/"$5"_*.xml)
for file in $upload_xmls
  do
    docker cp $file nesting:/root/models/nesting/simulations/schedplus/xml/"$1"/
  done
ini_file=$(ls $solution_dir/"$1"/"$2"/"$3"/"$4"/"$5"_*.ini)
docker cp $ini_file nesting:/root/models/nesting/simulations/schedplus/
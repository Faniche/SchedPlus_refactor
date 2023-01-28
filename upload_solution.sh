#!/bin/bash

solution_dir="cmake-build-debug/SolutionReport"

# $1: topology
# $2: streamNum
# $3: runId
# $4: solutionId
upload_xmls=$(ls $solution_dir/$1/$2/$3/$4_*.xml)
for file in $upload_xmls
  do
    docker cp $file nesting:/root/models/nesting/simulations/schedplus/xml/
  done
ini_file=$(ls $solution_dir/$1/$2/$3/$4_*.ini)
docker cp $ini_file nesting:/root/models/nesting/simulations/schedplus/
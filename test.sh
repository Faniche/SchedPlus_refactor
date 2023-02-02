#!/bin/bash

./SchedPlus -f 10 -t 1 -e 10 -g 50
./SchedPlus -f 20 -t 1 -e 10 -g 50
./SchedPlus -f 30 -t 1 -e 10 -g 50

./SchedPlus -f 10 -t 2 -e 10 -g 50
./SchedPlus -f 20 -t 2 -e 10 -g 50
./SchedPlus -f 30 -t 2 -e 10 -g 50

./SchedPlus -f 10 -t 3 -e 10 -g 50
./SchedPlus -f 20 -t 3 -e 10 -g 50
./SchedPlus -f 30 -t 3 -e 10 -g 50

./SchedPlus -f 10 -t 4 -e 10 -g 50
./SchedPlus -f 20 -t 4 -e 10 -g 50
./SchedPlus -f 30 -t 4 -e 10 -g 50


./SchedPlus -s SolutionReport/line_2sw_2es/10/line_2sw_2es_10_streams.json -t 1 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/line_2sw_2es/20/line_2sw_2es_20_streams.json -t 1 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/line_2sw_2es/30/line_2sw_2es_20_streams.json -t 1 -e 10 -g 50 -n

./SchedPlus -s SolutionReport/ring_4sw_8es/10/ring_4sw_8es_10_streams.json -t 2 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/ring_4sw_8es/20/ring_4sw_8es_20_streams.json -t 2 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/ring_4sw_8es/30/ring_4sw_8es_20_streams.json -t 2 -e 10 -g 50 -n

./SchedPlus -s SolutionReport/tree_7sw_21es/10/tree_7sw_21es_10_streams.json -t 3 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/tree_7sw_21es/20/tree_7sw_21es_20_streams.json -t 3 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/tree_7sw_21es/30/tree_7sw_21es_20_streams.json -t 3 -e 10 -g 50 -n

./SchedPlus -s SolutionReport/snow_7sw_18es/10/snow_7sw_18es_10_streams.json -t 4 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/snow_7sw_18es/20/snow_7sw_18es_20_streams.json -t 4 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/snow_7sw_18es/30/snow_7sw_18es_20_streams.json -t 4 -e 10 -g 50 -n

./SchedPlus -f 40 -t 2 -e 10 -g 50
./SchedPlus -f 50 -t 2 -e 10 -g 50

./SchedPlus -f 40 -t 3 -e 10 -g 50
./SchedPlus -f 50 -t 3 -e 10 -g 50

./SchedPlus -f 40 -t 4 -e 10 -g 50
./SchedPlus -f 50 -t 4 -e 10 -g 50


./SchedPlus -s SolutionReport/ring_4sw_8es/40/ring_4sw_8es_40_streams.json -t 2 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/ring_4sw_8es/50/ring_4sw_8es_50_streams.json -t 2 -e 10 -g 50 -n

./SchedPlus -s SolutionReport/tree_7sw_21es/40/tree_7sw_21es_40_streams.json -t 3 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/tree_7sw_21es/50/tree_7sw_21es_50_streams.json -t 3 -e 10 -g 50 -n

./SchedPlus -s SolutionReport/snow_7sw_18es/40/snow_7sw_18es_40_streams.json -t 4 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/snow_7sw_18es/50/snow_7sw_18es_50_streams.json -t 4 -e 10 -g 50 -n

./SchedPlus -f 40 -t 1 -e 10 -g 50
./SchedPlus -f 50 -t 1 -e 10 -g 50

./SchedPlus -s SolutionReport/line_2sw_2es/40/line_2sw_2es_40_streams.json -t 1 -e 10 -g 50 -n
./SchedPlus -s SolutionReport/line_2sw_2es/50/line_2sw_2es_50_streams.json -t 1 -e 10 -g 50 -n

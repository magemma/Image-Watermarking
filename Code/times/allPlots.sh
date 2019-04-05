#!/bin/bash

#Usage: ./allPlots.sh outDell timesDell.txt 500
declare -a PROG

FLD=$1
IN=$2
NIMG=$3
PROGS=$(tail -n +2 $IN | cut -f 1 -d ' ' | sort | uniq)

WMS=$(tail -n +2 $IN | cut -f 4 -d ' ' | sort | uniq)

for wm in $WMS
do
  FILES=""
  for p in $PROGS
  do
    if echo $p | grep -v sequential > /dev/null
    then
    	#cat $p
      PROG+=($p)
      FILES="$FILES $FLD/$p/$wm/speedup.dat"
    fi
  done
  #echo $FILES
  paste $FILES > $FLD/$wm-all.dat
  gnuplot << EOF
  set term postscript enhanced color
  set output "$FLD/plots.pdf"

  set style line 1 \
    linecolor rgb '#c36cff' \
    linetype 1 linewidth 2

  set style line 2 \
    linecolor rgb '#c36cff' \
    dashtype 2 linewidth 2
    
  set style line 3 \
    linecolor rgb '#490063' \
    linetype 1 linewidth 2

  set style line 4 \
    linecolor rgb '#490063' \
    dashtype 2 linewidth 2

  set style line 5 \
    linecolor rgb '#dfa6a6' \
    linetype 1 linewidth 2
  
  set style line 6 \
    linecolor rgb '#dfa6a6' \
    dashtype 2 linewidth 2
  
  set style line 7 \
    linecolor rgb '#8d021f' \
    linetype 1 linewidth 2

  set style line 8 \
    linecolor rgb '#8d021f' \
    dashtype 2 linewidth 2

  set style line 9 \
    linecolor rgb '#567865' \
    linetype 1 linewidth 2

  set style line 10 \
    linecolor rgb '#dfa6a6' \
    linetype 2 linewidth 2
	set key outside

  plot "$FLD/$wm-all.dat" using 1:2 with lines linestyle 1 title "${PROG[0]}", \
  "$FLD/$wm-all.dat" using 1:4 with lines linestyle 2 title "${PROG[1]}", \
  "$FLD/$wm-all.dat" using 1:6 with lines linestyle 3 title "${PROG[2]}", \
  "$FLD/$wm-all.dat" using 1:8 with lines linestyle 4 title "${PROG[3]}", \
  "$FLD/$wm-all.dat" using 1:10 with lines linestyle 5 title "${PROG[4]}", \
  "$FLD/$wm-all.dat" using 1:12 with lines linestyle 6 title "${PROG[5]}", \
  "$FLD/$wm-all.dat" using 1:14 with lines linestyle 7 title "${PROG[6]}", \
  "$FLD/$wm-all.dat" using 1:16 with lines linestyle 8 title "${PROG[7]}"



 


EOF
done


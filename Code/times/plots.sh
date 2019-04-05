#!/bin/bash

#Usage: ./plots.sh timesDell.txt 500
IN=$1
NIMG=$2
[ -z "$NIMG" ] && exit 1

PROGS=$(tail -n +2 $IN | cut -f 1 -d ' ' | sort | uniq)
WMS=$(tail -n +2 $IN | cut -f 4 -d ' ' | sort | uniq)


for p in $PROGS
do
  if echo $p | grep Memory &>/dev/null
  then
    SEQ=sequentialMemory
  else
    SEQ=sequential
  fi
  for w in $WMS
  do
    DIR=out/$p/$w
    mkdir -p $DIR
    > $DIR/speedup.dat
    > $DIR/scalability.dat
    > $DIR/all.dat
    SEQT=$(cat $1 | grep "$SEQ 1 $NIMG $w" | cut -f 5 -d ' ') 
    T1TH=$(cat $1 | grep "$p 1 $NIMG $w" | cut -f 5 -d ' ') 
    grep $p $1 | while read P PAR NI W T TT
    do
      if [ "$NIMG" != "$NI" ]; then continue; fi
      if [ "$w" != "$W" ]; then continue; fi
      if [ "$p" != "$P" ]; then continue; fi
      SPE=$(bc -l <<< $SEQT/$T)
      SCA=$(bc -l <<< $T1TH/$T)
      echo $PAR $SPE >> $DIR/speedup.dat
      echo $PAR $SCA >> $DIR/scalability.dat
      echo $PAR $SPE $SCA >> $DIR/all.dat
    done
  done
done

for SPE in $(find out -name 'speedup.dat')
do
  DST=$(echo $SPE | sed s/.dat/.eps/)
  gnuplot << EOF
set term postscript enhanced color
set output "$DST"

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2 

plot "$SPE" with lines linestyle 1 title "speedup"
EOF
done

for SCA in $(find out -name 'scalability.dat')
do
  DST=$(echo $SCA | sed s/.dat/.eps/)
  gnuplot << EOF
set term postscript enhanced color
set output "$DST"

set style line 1 \
    linecolor rgb '#dd181f' \
    linetype 1 linewidth 2 

plot "$SCA" with lines linestyle 1 title "scalability"
EOF
done

for ALL in $(find out -name 'all.dat')
do
  DST=$(echo $ALL | sed s/.dat/.eps/)
  gnuplot << EOF
set term postscript enhanced color
set output "$DST"

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2 
set style line 2 \
    linecolor rgb '#dd181f' \
    linetype 1 linewidth 2 

plot "$ALL" using 1:2 with lines linestyle 1 title "speedup", \
     "$ALL" using 1:3 with lines linestyle 2 title "scalability"
EOF
done

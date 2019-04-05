#!bin/bash/

sourceFile="stats.txt"
countLines=0
avgtLoadWM=0
avgtProWM=0
avgtList=0
avgtLoad=0
avgtCalc=0
avgtSave=0

while read tLoadWM tProWM tList tLoad tCalc tSave; do
  let countLines=1+$countLines
  if [ $countLines -gt 2 ]
  then
    let avgtLoadWM=$avgtLoadWM+$tLoadWM
    let avgtProWM=$avgtProWM+$tProWM
    let avgtList=$avgtList+$tList
    let avgtLoad=$avgtLoad+$tLoad
    let avgtCalc=$avgtCalc+$tCalc
    let avgtSave=$avgtSave+$tSave
    #echo $tLoadWM;
  fi
done < $sourceFile
let countLines=$countLines-2
let avgtLoadWM=$avgtLoadWM/$countLines
let avgtProWM=$avgtProWM/$countLines
let avgtList=$avgtList/$countLines
let avgtLoad=$avgtLoad/$countLines
let avgtCalc=$avgtCalc/$countLines
let avgtSave=$avgtSave/$countLines
 

echo "Number of sample lines: " $countLines

echo "Avg time to load the watermark (usec): " $avgtLoadWM
echo "Avg time to process the watermark (usec): " $avgtProWM
echo "Avg time to compute the list of the paths of the images (usec): " $avgtList
echo "Avg time to load an image (usec): " $avgtLoad
echo "Avg time to compute the watermarking on an image (usec): " $avgtCalc
echo "Avg time to store the new image (usec): " $avgtSave




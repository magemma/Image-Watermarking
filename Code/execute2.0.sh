#!/usr/bin/env bash

folder=times
#Creating folder for time results
mkdir -p $folder

for wm in $(ls ../Images/wm)
do
  echo "~~~~~~~~~~~~~~~~~~~~Using watermark $wm~~~~~~~~~~~~~~~~~~~"

  echo "~~~~~~~~Executing sequential~~~~~~~~~~~~~~~~~~~"
  #LD_LIBRARY_PATH=/usr/local/lib64
  ./sequential ../Images/input/ ../Images/wm/$wm ../Images/output/

  echo "~~~~~~~~Executing sequentialMemory~~~~~~~~~~~~~"
  #LD_LIBRARY_PATH=/usr/local/lib64
  ./sequentialMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ 
  
  for parDegree in 1 2 4 #8 16 32 64 128 256
	do 
    
    rm -rf outputs
    mkdir -p outputs
    
    echo "~~~~~~Executing $parDegree-parallelFor~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./parallelFor ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree
    
    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-parallelForMemory~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./parallelForMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-fastFlow~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./fastFlow ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-fastFlowMemory~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./fastFlowMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-threads~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./threads ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-threadsMemory~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./threadsMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 
    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-grPPi~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./grPPi ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 
    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~Executing $parDegree-grPPiMemory~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    ./grPPiMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree
	done
done

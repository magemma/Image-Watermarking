#!/usr/bin/env bash

#On xeonPhi decomment following
#folder=timesPhi
folder=times
#Creating folder for time results
mkdir -p $folder/$wm

for wm in $(ls ../Images/wm)
do
  echo "~~~~~~~~~~~~~~~~~~~~Using watermark $wm~~~~~~~~~~~~~~~~~~~"
  #Creating folder for time results
  mkdir -p $folder/$wm

  echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing sequential~~~~~~~~~~~~~~~~~~"
  #LD_LIBRARY_PATH=/usr/local/lib64
  /usr/bin/time ./sequential ../Images/input/ ../Images/wm/$wm ../Images/output/ 2>&1 | tee $folder/$wm/time-sequential.txt

  echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing sequentialMemory~~~~~~~~~~~~~~~~~~"
  #LD_LIBRARY_PATH=/usr/local/lib64
  /usr/bin/time ./sequentialMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ 2>&1 | tee $folder/$wm/time-sequentialMemory.txt

  for parDegree in 1 2 4 #8 16 32 64 128 256
	do 
    
    rm -rf outputs
    mkdir -p outputs
    
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-parallelFor~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    OMP_NUM_THREADS=$parDegree /usr/bin/time ./parallelFor ../Images/input/ ../Images/wm/$wm ../Images/output/ 2>&1 | tee $folder/$wm/time-parallelFor-$parDegree.txt
    
    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-parallelForMemory~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    OMP_NUM_THREADS=$parDegree /usr/bin/time ./parallelForMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ 2>&1 | tee $folder/$wm/time-parallelForMemory-$parDegree.txt

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-fastFlow~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./fastFlow ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-fastFlow-$parDegree.txt

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-fastFlowMemory~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./fastFlowMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-fastFlowMemory-$parDegree.txt

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-threads~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./threads ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-threads-$parDegree.txt

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-threadsMemory~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./threadsMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-threadsMemory-$parDegree.txt
    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-grPPi~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./grPPi ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-grPPi-$parDegree.txt

    rm -rf outputs
    mkdir -p outputs

    echo "~~~~~~~~~~~~~~~~~~~~~~~~~Executing $parDegree-grPPiMemory~~~~~~~~~~~~~~~~~~"
    #LD_LIBRARY_PATH=/usr/local/lib64
    /usr/bin/time ./grPPiMemory ../Images/input/ ../Images/wm/$wm ../Images/output/ $parDegree 2>&1 | tee $folder/$wm/time-grPPiMemory-$parDegree.txt
	done
done

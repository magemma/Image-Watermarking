#
# image watermarking SPM 2018 
# 
# Dipartimento di Informatica Università di Pisa
#
# Studentessa: Martini Gemma
# 
# Docenti: Danelutto, Torquati
#
#

INCLUDE_FILES = CImg.h 

DEPENDENCIES =  dependencies.cpp

CC = g++ #PATH=/bin:/usr/bin:/opt/intel/bin icc #g++

#CFLAGS += -std=c++11 

CFLAGS += -Wall -pedantic -g -Dcimg_display=0 -Dcimg_use_jpeg=1 -Dcimg_use_png=1 -ljpeg -lpng -lz -pthread #-fsanitize=address

OPTFLAGS	= -O3 -march=native

OPENMPFLAGS = -fopenmp

all: sequential sequentialMemory parallelFor parallelForMemory threads threadsMemory fastFlow fastFlowMemory grPPi grPPiMemory

#sequentialTimes
cleanup: 
	rm sequential sequentialTimes parallelFor parallelForMemory threads fastFlow fastFlowMemory grPPi

sequential: sequential.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) sequential.cpp -o sequential
	
sequentialMemory: sequentialMemory.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) sequentialMemory.cpp -o sequentialMemory

sequentialTimes: sequentialTimes.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) sequentialTimes.cpp -o sequentialTimes

parallelFor: parallelFor.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(OPENMPFLAGS) $(DEPENDENCIES) parallelFor.cpp -o parallelFor 

parallelForMemory: parallelForMemory.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(OPENMPFLAGS) $(DEPENDENCIES) parallelForMemory.cpp -o parallelForMemory 

threads: threads.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) threads.cpp -o threads 

threadsMemory: threadsMemory.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) threadsMemory.cpp -o threadsMemory 

fastFlow: fastFlow.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) -I . -faligned-new fastFlow.cpp -o fastFlow

fastFlowMemory: fastFlowMemory.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) -I . -faligned-new fastFlowMemory.cpp -o fastFlowMemory

grPPi: grPPi.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) -I . -faligned-new -I include/ grPPi.cpp -o grPPi

grPPiMemory: grPPiMemory.cpp $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPENDENCIES) -I . -faligned-new -I include/ grPPiMemory.cpp -o grPPiMemory

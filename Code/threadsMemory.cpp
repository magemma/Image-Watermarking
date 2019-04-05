
/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this is a parallel implementation of map pattern using c++
 * threads
 *
 * Example of usage: ./threadsMemory ../Images/input/
 ../Images/wm/blotches-01.png
 ../Images/output/ par_degree
 */
#include <chrono>
using namespace std::chrono;

#include "dependencies.h"

// Global vector of worker threads
vector<thread> threads;

int main(int argc, char *argv[]) {
	high_resolution_clock::time_point start = high_resolution_clock::now();
  if (argc < 5) {
    errno = EINVAL;
    perror("Not enough arguments");
    gentleQuit();
  }
  if (argc > 5) {
    errno = EINVAL;
    perror("Too many arguments");
    gentleQuit();
  }
  allINeed everything;
  string name = argv[0];
  name = name.substr(2, name.size()-1);
  everything.imgPath = argv[1];
  everything.waterPath = argv[2];
  everything.newDirPath = argv[3];
  int parDegree = atoi(argv[4]);
  //(1) Reading watermark
  CImg<unsigned char> waterM = loadPic(everything.waterPath);
  //(2) Writing the watermark in a vector
  everything.wm = processWatermark(waterM);
  //(3) Getting a list of CImgs in RAM
  vector<CImg<unsigned char>> pictures;
  memoryPreProcessing(everything, pictures);
  int numPics = pictures.size();
  //(4) Initialize counter to 0
  atomic<int> counter{0};
  //(4bis) Define behaviour of threads
  auto myJob = [&]() {
    while (true) {
      int myId = counter++;
      if (myId >= numPics)
        break;
      // For each image we edit each pixel
      for (int j = 0; j < (int)everything.wm.size(); j++) {
        // For each pixel of the watermark
        editImage(everything, pictures[myId], j);
      }
    }
  };
  //(5) Start the requested number of threads
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  for (int j = 0; j < parDegree; j++) {
    threads.push_back(std::thread(myJob));
  }
  //(6) Join threads
  for (int j = 0; j < parDegree; j++) {
    threads[j].join();
  }
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  int computationElapsed = duration_cast<microseconds>(stop - computationStart).count();
  int totalElapsed = duration_cast<microseconds>(stop - start).count();
  string printWM = everything.waterPath.substr(13, everything.waterPath.size()-4);
  printTimes(name, parDegree, numPics, printWM, computationElapsed, totalElapsed);
  return 0;
}

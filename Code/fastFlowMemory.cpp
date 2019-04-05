
/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this is an implementation of map pattern using fastflow parallel
 * for. In this implementation we avoid any kind of access to the disk during
 * the edit operations.
 *
 * Example of usage: ./fastFlowMemory ../Images/input/
 * ../Images/wm/blotches-01.png
 * ../Images/output/ par_degree
 */

#include "dependencies.h"
#include <chrono>
using namespace std::chrono;

// Include FastFlow parallel loop
#include "ff/parallel_for.hpp"
using namespace ff;

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
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  //(4) We edit each image without storing the result
  ParallelFor pf(parDegree);
  pf.parallel_for(0, numPics, [&everything, &pictures](int i) {
    // For each image
    for (int j = 0; j < (int)everything.wm.size(); j++) {
      // For each pixel of the watermark
      editImage(everything, pictures[i], j);
    }
  });
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  int computationElapsed = duration_cast<microseconds>(stop - computationStart).count();
  int totalElapsed = duration_cast<microseconds>(stop - start).count();
  string printWM = everything.waterPath.substr(13, everything.waterPath.size()-4);
  printTimes(name, parDegree, numPics, printWM, computationElapsed, totalElapsed);
  return 0;
}

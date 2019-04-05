
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
 * for
 *
 * Example of usage: ./fastFlow ../Images/input/ ../Images/wm/blotches-01.png
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
  //(3) Making list of the images in the path
  everything.allImages = listImages(everything);
  int numPics = everything.allImages.size();
  //(4) Creating a directory for the new images
  mkdir(everything.newDirPath.c_str(), S_IRWXU);
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  //(5) For each path in allImages
  ParallelFor pf(parDegree);
  int iterations = (int)everything.allImages.size();
  pf.parallel_for(0, iterations,
                  [&everything](int i) { forEachImage(everything, i); });
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  int computationElapsed = duration_cast<microseconds>(stop - computationStart).count();
  int totalElapsed = duration_cast<microseconds>(stop - start).count();
  string printWM = everything.waterPath.substr(13, everything.waterPath.size()-4);
  printTimes(name, parDegree, numPics, printWM, computationElapsed, totalElapsed);
  return 0;
}

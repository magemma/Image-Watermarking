
/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this file implements a parallel execution
 *
 * Example of usage: ./sequentialMemory ../Images/input/
 * ../Images/wm/blotches-01.png
 * ../Images/output/
 */
#include <chrono>
using namespace std::chrono;

#include "dependencies.h"

int main(int argc, char *argv[]) {
high_resolution_clock::time_point start = high_resolution_clock::now();
  if (argc < 4) {
    errno = EINVAL;
    perror("Not enough arguments");
    gentleQuit();
  }
  if (argc > 4) {
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
  //(1) Reading watermark
  CImg<unsigned char> waterM = loadPic(everything.waterPath);
  //(2) Writing the watermark in a vector
  everything.wm = processWatermark(waterM);
  //(3) Making list of the images in the path
  everything.allImages = listImages(everything);
  int numPics = everything.allImages.size();
  //(4) Creating a directory for the new images
  mkdir(everything.newDirPath.c_str(), S_IRWXU);
  //(5) For each path in allImages
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  for (int i = 0; i < (int)everything.allImages.size(); i++) {
    forEachImage(everything, i);
  }
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  int computationElapsed = duration_cast<microseconds>(stop - computationStart).count();
  int totalElapsed = duration_cast<microseconds>(stop - start).count();
  string printWM = everything.waterPath.substr(13, everything.waterPath.size()-4);
	printTimes(name, 1, numPics, printWM, computationElapsed, totalElapsed);
  return 0;
}

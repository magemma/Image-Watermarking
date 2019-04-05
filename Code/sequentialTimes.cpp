/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this file computes the times nedded for performing the different
 * operations
 */

#include "dependencies.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char *argv[]) {
  high_resolution_clock::time_point tLoadWM1 = high_resolution_clock::now();
  if (argc < 4) {
    errno = EINVAL;
    perror(
        "Not enough arguments. You should run the code specifying the path to "
        "the folder containing the images to be watermarked, the path to the "
        "watermark and the path of the outputfolder\n For example: ./wmTimes ");
    gentleQuit();
  }
  if (argc > 4) {
    errno = EINVAL;
    perror("Too many arguments");
    gentleQuit();
  }
  FILE *myFd = fopen("stats.txt", "a+");
  allINeed everything;
  everything.imgPath = argv[1];
  everything.waterPath = argv[2];
  everything.newDirPath = argv[3];
  CImg<unsigned char> waterM = loadPic(everything.waterPath);
  high_resolution_clock::time_point tLoadWM2 = high_resolution_clock::now();
  tLoadWM = duration_cast<microseconds>(tLoadWM2 - tLoadWM1).count();
  //(2) Writing the watermark in a vector
  high_resolution_clock::time_point tProcessWM1 = high_resolution_clock::now();
  everything.wm = processWatermark(waterM);
  high_resolution_clock::time_point tProcessWM2 = high_resolution_clock::now();
  tProcessWM = duration_cast<microseconds>(tProcessWM2 - tProcessWM1).count();
  //(3) Making list of the images in the path
  high_resolution_clock::time_point tList1 = high_resolution_clock::now();
  everything.allImages = listImages(everything);
  high_resolution_clock::time_point tList2 = high_resolution_clock::now();
  tList = duration_cast<microseconds>(tList2 - tList1).count();
  //(4) Creating a directory for the new images
  mkdir(everything.newDirPath.c_str(), S_IRWXU);
  //(5) For each path in allImages
  int iterations = (int)everything.allImages.size();
  for (int i = 0; i < iterations; i++) {
    forEachImage(everything, i);
    writeLine(myFd);
  }
  return 0;
}

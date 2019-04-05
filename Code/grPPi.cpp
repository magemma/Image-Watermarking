
/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this is an implementation of stream parallelism using GrpPPI
 * framework
 *
 * Example of usage: ./grPPi ../Images/input/ ../Images/wm/blotches-01.png
 * ../Images/output/ parallelism_degree
 */
#include <chrono>
using namespace std::chrono;

#include "dependencies.h"
#include <experimental/optional>
#include <grppi.h>
using namespace grppi;

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

  auto genstream = [&]() -> experimental::optional<int> {
    static int i = 0;
    if (i < (int)everything.allImages.size())
      return (i++);
    else
      return {};
  };

  auto lambdaForEachImage = [&](int i) -> int {
    forEachImage(everything, i);
    return i;
  };

  auto fakedrain = [&](int i) {
    // cout << "Image " << everything.allImages[i].first << "/"
    //<< everything.allImages[i].second << " has been processed" << endl;
    // No printing because that would be annoying for experiments
  };
  //(5) For each path in allImages
  // grppi pipeline with second stage (farm)
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  dynamic_execution thr = grppi::parallel_execution_native{};
  pipeline(thr, genstream, farm(parDegree, lambdaForEachImage), fakedrain);
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  int computationElapsed = duration_cast<microseconds>(stop - computationStart).count();
  int totalElapsed = duration_cast<microseconds>(stop - start).count();
  string printWM = everything.waterPath.substr(13, everything.waterPath.size()-4);
  printTimes(name, parDegree, numPics, printWM, computationElapsed, totalElapsed);
  return 0;
}

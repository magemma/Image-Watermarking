
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
 * Example of usage: ./threads ../Images/input/ ../Images/wm/blotches-01.png
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
  //(3) Making list of the images in the path
  everything.allImages = listImages(everything);
  int numPics = everything.allImages.size();
  //(4) Creating a directory for the new images
  mkdir(everything.newDirPath.c_str(), S_IRWXU);
  //(5) Initialize counter to 0
  atomic<int> counter{0};
  //(5bis) Define behaviour of threads
  auto myJob = [&counter, &everything]() {
    while (true) {
      int myId = counter++;
      if (myId >= (int)everything.allImages.size())
        break;
      forEachImage(everything, myId);
    }
  };
  high_resolution_clock::time_point computationStart = high_resolution_clock::now();
  //(6) Start the requested number of threads
  for (int j = 0; j < parDegree; j++) {
    threads.push_back(std::thread(myJob));
  }
  //(7) Join threads
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

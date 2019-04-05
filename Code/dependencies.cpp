
/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this file contains all the auxiliary functions for allowing
 * different parallel executions
 */

#include "dependencies.h"

// Global variables for counting time
long int tLoadWM;
long int tProcessWM;
long int tList;
long int tLoad;
long int tCalc;
long int tSave;

/**
 * @function gentleQuit
 * @brief this function kills the program if there is something wrong
 */
void gentleQuit() { exit(1); }

/**
 * @function checkSize
 * @brief this function calls gentleQuit in case of image of different size from
 * the one that was defined in the global variable
 *
 * @param img image which size needs to be checked
 */
void checkSize(const CImg<unsigned char> &img) {
  if ((img.width() != width) || (img.height() != height))
    gentleQuit();
  return;
}

/**
 * @procedure writeLine
 * @brief writes the four global variables in a file. It is needed by
 * wmTimes.cpp only
 *
 * @param myFd file where to write
 */
void writeLine(FILE *myFd) {
  fprintf(myFd, "%ld \t %ld \t %ld \t %ld \t %ld \t %ld\n", tLoadWM, tProcessWM,
          tList, tLoad, tCalc, tSave);
}

/**
 * @function loadPic
 * @brief this function opens a picture from its path
 *
 * @param path which is the full path of the watermark
 *
 * @return image, which is the image from the path
 */
CImg<unsigned char> loadPic(string &path) {
  const CImg<unsigned char> image = CImg<>(path.c_str());
  checkSize(image);
  return image;
}

/**
 * @function processWatermark
 * @brief this function makes a vector out of the watermark
 *
 * @param waterM the CImg of the watermark
 *
 * @return wm, the vector of pairs that represent the position of
 *          black pixels in the watermark
 */
vector<pair<int, int>> processWatermark(CImg<unsigned char> &waterM) {
  //cout << "Watermark processing\n";
  vector<pair<int, int>> wm;
  int width = waterM.width();
  int height = waterM.height();
  // Our watermark has a spectrum of 1, but this way is more flexible
  int spectrum = waterM.spectrum();
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      bool isBlack = true;
      for (int k = 0; k < spectrum; k++) {
        int v = (int)waterM(i, j, 0, k);
        if (v != 0)
          isBlack = false;
      }
      if (isBlack) {
        pair<int, int> coordinates = make_pair(i, j);
        wm.push_back(coordinates);
      }
    }
  }
  return wm;
}

/**
 * @function listImages
 * @brief this function makes a list of paths for each image in the
 *		directory referred by path
 *
 * @param everything, reference to the  data structure that contains
 *		all the important vectors and paths, like imgPath, the path to
 *		the folder containing the pictures to be edited
 *
 * @return myList, the list of complete paths of all the images
 */
vector<pair<string, string>> listImages(allINeed &everything) {
  //cout << "Listing images\n";
  string path = everything.imgPath;
  DIR *pDIR;
  struct dirent *entry;
  vector<pair<string, string>> myList;
  if ((pDIR = opendir(path.c_str()))) {
    while ((entry = readdir(pDIR))) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        string fullPath = path + entry->d_name;
        pair<string, string> elem = make_pair(fullPath, entry->d_name);
        myList.push_back(elem);
      }
    }
  }
  closedir(pDIR);
  return myList;
}

/**
 * @function editImage
 * @brief this function edits a given position of the matrix
 *
 * @param everything the data structure
 *
 * @param currImg the pointer to the image to edit
 *
 * @param indexWM where everything.wm[indexWM] contains the
 *    coordinates of the pixel to edit
 * */
void editImage(allINeed &everything, CImg<unsigned char> &currImg,
               int indexWM) {
  int x = everything.wm[indexWM].first;
  int y = everything.wm[indexWM].second;
  int spectrum = currImg.spectrum();
  int count = 0;
  for (int i = 0; i < spectrum; i++) {
    count += currImg(x, y, 0, i);
  }
  int newVal = count / (spectrum * 2);
  for (int i = 0; i < spectrum; i++) {
    currImg(x, y, 0, i) = newVal;
  }
}

/**
 * @function storeImage
 * @brief this function stores the new image on disk
 *
 * @param currImg pointer to the image to be stored
 *
 * @param index index of the image in everything.allImages
 *
 * @param everything the data structure
 * */
void storeImage(CImg<unsigned char> &currImg, int index, allINeed &everything) {
  string newName = everything.newDirPath + everything.allImages[index].second;
  currImg.save(newName.c_str());
}

/**
 * @function forEachImage
 * @brief this function loads the image in main memory, then reads the whole
 * vector wm and, for each position pos: (1) img[pos] =(img.R + img.G +
 * img.B)/6; (2) writes the image on the disc
 *
 * @param everything the data structure that contains all images and
 *                  the vector which represents the watermark
 * @param index the number of images processed so far
 */
void forEachImage(allINeed &everything, int index) {
  //if (index % 100 == 0)
    //cout << "Processing image number " << index << "\n";
  CImg<unsigned char> currImg = loadPic(everything.allImages[index].first);
  for (int i = 0; i < (int)everything.wm.size(); i++) {
    editImage(everything, currImg, i);
  }
  storeImage(currImg, index, everything);
  return;
}

/**
 * @function memoryPreProcessing
 * @brief this function counts the number of images contained in the input
 * folder, loads one of them and copies that image in RAM as many times as the
 * amount of images contained in the input folder
 *
 * @param everything the data structure that contains all images and
 *                  the vector which represents the watermark
 * @param pictures the vector of CImgs
 */
void memoryPreProcessing(allINeed &everything,
                         vector<CImg<unsigned char>> &pictures) {
  // Computing the number of images in the directory everything.imgPath
  //cout << "Loading the list of images in RAM\n";
  string path = everything.imgPath;
  DIR *pDIR;
  struct dirent *entry;
  int numPics = 0;
  string picPath;
  if ((pDIR = opendir(path.c_str()))) {
    while ((entry = readdir(pDIR))) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        numPics++;
        // Loading one image from the folder
        picPath = path + entry->d_name;
      }
    }
    closedir(pDIR);
  }
  CImg<unsigned char> img = loadPic(picPath);
  //(5) We copy in memory the image numPics times
  for (int i = 0; i < numPics; i++) {
    pictures.push_back(CImg<unsigned char>(img));
  }
  return;
}

void printTimes(string name, int parDegree, int numPics, string printWM, int computationElapsed, int totalElapsed) {
//Careful! Very hardcoded in the struture of the folders
  FILE * pFile;
  pFile = fopen (timesPath.c_str(),"a+");
  if (pFile!=NULL) {
  	fprintf(pFile, "%s %d %d %s %d %d\n", name.c_str(), parDegree, numPics, printWM.c_str(), computationElapsed, totalElapsed);
    fclose (pFile);
  }
  return;
}


/*
 * Image Watermarking SPM 2018
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Martini Gemma
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this file defines all the auxiliary functions for allowing
 * different parallel executions
 */
#pragma once
#include <atomic>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

// Include CImg library header.
#include "CImg.h"

using namespace cimg_library;
using namespace std;
// Global variables for checking size
static constexpr int height = 1080;
static constexpr int width = 1920;

// Global variables for counting time
extern long int tLoadWM;
extern long int tProcessWM;
extern long int tList;
extern long int tLoad;
extern long int tCalc;
extern long int tSave;

const string timesPath = "times/timesDell.txt";
//static constexpr string timesPath = "times/timesPhi.txt";

typedef struct stuff {
  vector<pair<string, string>> allImages;
  vector<pair<int, int>> wm;
  string waterPath;
  string imgPath;
  string newDirPath;
} allINeed;

/**
 * @function gentleQuit
 * @brief this function kills the program if there is something wrong
 */
void gentleQuit();

/**
 * @function checkSize
 * @brief this function calls gentleQuit in case of image of different size from
 * the one that was defined in the global variable
 *
 * @param img image which size needs to be checked
 */
void checkSize(const CImg<unsigned char> &img);

/**
 * @procedure writeLine
 * @brief writes the four global variables in a file. It is needed by
 * wmTimes.cpp only
 *
 * @param myFd file where to write
 */
void writeLine(FILE *myFd);

/**
 * @function loadPic
 * @brief this function opens a picture from its path
 *
 * @param path which is the full path of the watermark
 *
 * @return image, which is the image from the path
 */
CImg<unsigned char> loadPic(string &path);

/**
 * @function processWatermark
 * @brief this function makes a vector out of the watermark
 *
 * @param waterM the CImg of the watermark
 *
 * @return wm, the vector of pairs that represent the position of
 *          black pixels in the watermark
 */
vector<pair<int, int>> processWatermark(CImg<unsigned char> &waterM);

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
vector<pair<string, string>> listImages(allINeed &everything);

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
void editImage(allINeed &everything, CImg<unsigned char> &currImg, int indexWM);

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
void storeImage(CImg<unsigned char> &currImg, int index, allINeed &everything);

/**
 * @function forEachImage
 * @brief this function reads the whole vector wm and, for each
 *    position pos:
 *      (1) img[pos] =(img.R + img.G + img.B)/6;
 *      (2) writes the image on the disc
 *
 * @param everything the data structure that contains all images and
 *                  the vector which represents the watermark
 * @param index the number of images processed so far
 */
void forEachImage(allINeed &everything, int index);

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
                         vector<CImg<unsigned char>> &pictures);
                         
void printTimes(string name, int parDegree, int numPics, string printWM, int computationElapsed, int totalElapsed);

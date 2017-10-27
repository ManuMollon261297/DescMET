
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "lodepng.h"
#include "boost\filesystem.hpp"

#define EXTENSION ".png"
namespace fs = boost::filesystem;

bool decompressImage(const char * imagePath);

void recursiveDrawing(unsigned int width_, fs::ifstream& readFile, std::vector<unsigned int>& nVector, std::vector<unsigned char>& pngImage);

void printRGBA(std::vector<unsigned int>& nVector, unsigned int width, std::vector<unsigned char>& pngImage, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

void getCorr(unsigned int& corrX, unsigned int& corrY, std::vector<unsigned int>& nVector, unsigned int width);


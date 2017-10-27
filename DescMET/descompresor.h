
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "lodepng.h"
#include "boost\filesystem.hpp"

#define EXTENSION ".png"

namespace fs = boost::filesystem;

bool decompressImage(const char * imagePath);

// width se refiere al largo original de la imagen,
// readFile es el .met
// nVector es un vector donde se van cuardando las veces que se dividio el archivo y en que cuadrante se encuentra en ese nivel
// pngImage sera luego encodeado en png por la libreria lodepng
void recursiveDrawing(unsigned int width_, fs::ifstream& readFile, std::vector<unsigned int>& nVector, std::vector<unsigned char>& pngImage);

void printRGBA(std::vector<unsigned int>& nVector, unsigned int width, std::vector<unsigned char>& pngImage, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

//Asume que corrX y corrY estan inicializados en 0
void getCorr(unsigned int& corrX, unsigned int& corrY, std::vector<unsigned int>& nVector, unsigned int width);


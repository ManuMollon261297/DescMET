#include "descompresor.h"

bool decompressImage(const char * imagePath)
{
	std::vector<unsigned int> nVector;
	std::vector<unsigned char> bmpImage;

	std::string imageName(imagePath);
	imageName.resize(imageName.size() - 4); //remuevo el ".met" del nombre
	std::string extension = EXTENSION;
	std::string pngFile(imageName + extension); //creo el archivo
	fs::ifstream bmpFile(imagePath, std::ios_base::in | std::ios_base::binary);

	//extraer los primeros cuatro bytes del width de la imagen
	unsigned int width;
	unsigned char lengthAux[4];
	for (int j = 0; j < 4; j++)			//Tomo los 4 primeros bytes del archivo que representan
	{									//el largo de la imagen original en pixeles
		lengthAux[j] = bmpFile.get();
	}
	unsigned int * tempPointer = (unsigned int *)lengthAux;
	width = *tempPointer;
	std::cout << "Image Width: " << width << std::endl;
	bmpImage.resize((size_t)(width*width * 4));

	recursiveDrawing(width, bmpFile, nVector, bmpImage);
	unsigned error = lodepng::encode(pngFile, bmpImage, width, width);
	if (error)
	{
		std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return true;
	}
	return false;
}


void recursiveDrawing(unsigned int width, fs::ifstream& readFile, std::vector<unsigned int>& nVector, std::vector<unsigned char>& pngImage)
{
	unsigned char currentValue = 0; //char que se va leyendo el archivo
	unsigned char red, green, blue;
	currentValue = readFile.get();
	while(!readFile.eof()) // se dibuja hasta llegar al eof
	{
		if (((char)currentValue) == 'H') 
		{
			nVector.push_back(0); // como hay una 'H' la imagen se dividio en 4 cuadrantes
		}
		else if (((char)currentValue) == 'N') // Si leo una 'N', se que le siguen tres colores
		{
			red = readFile.get();
			green = readFile.get();
			blue = readFile.get();
			if (nVector.size() != 0)
			{
				nVector[nVector.size() - 1]++;	// mientras no este en el cuadrante original (size = 0) caso especial
												// donde solo hay un cuadrante, incremento el contador de cuadrantes
												// de el nivel en el que me encuentro
			}
			printRGBA(nVector, width, pngImage, red, green, blue, 255); // dibujo el cuadrante correspondiente
			while ((nVector.size() != 0) && ((nVector[nVector.size() - 1] == 4)))	// si se dibujo los cuatro cuadrantes correspondientes
			{																		// a el cuadrado del nivel superior, borro la n
																					// correspondiente a ese cuadrado y paso al siguiente 
				nVector.pop_back();													// cuadrante, haci hasta que no sea cuatro el nivel actual
				if (nVector.size() != 0)
				{
					nVector[nVector.size() - 1]++;
				}
			}
		}
		currentValue = readFile.get(); // agarro el proximo char, si llego al eof, salta en la proxima condiciond del while
	}
}

void printRGBA(std::vector<unsigned int>& nVector,unsigned int width, std::vector<unsigned char>& pngImage, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	unsigned int corrX = 0;
	unsigned int corrY = 0;
	unsigned int actualWidth = width / pow(2, nVector.size());	// largo del cuadrado que voy a dibjar, calculado en base a el nivel en el
																// que me encuentro
	getCorr(corrX, corrY, nVector, width); //busco el (0,0) relativo para ese cuadrante 

	unsigned int  iFinal, jFinal;
	iFinal = corrX  + (actualWidth)*4; // limites en x e y para dibujar el cuadrado
	jFinal = corrY + (actualWidth);

	for (unsigned int i = corrX; i < iFinal; i+=4) //for que me dibuja el cuadrado correspondiente y agrega el alpha arbitrario
	{
		for (unsigned int j = corrY; j < jFinal; j++)
		{
			pngImage[i + j*(width * 4)] = red;
			pngImage[i + j*(width * 4) + 1] = green;
			pngImage[i + j*(width * 4) + 2] = blue;
			pngImage[i + j*(width * 4) + 3] = alpha;
		}
	}
}

void getCorr(unsigned int& corrX, unsigned int& corrY, std::vector<unsigned int>& nVector, unsigned int width)
{
	for (int currentDepth = 1; currentDepth <= nVector.size(); currentDepth++) // va sumando los corrimientos para cad anivel hasta llegar al actual
	{
			unsigned int switchCase;
			if (nVector.size() == currentDepth)
			{
				switchCase = nVector[currentDepth - 1]; // para el cuadrante actual, el valor en n sera igual al cuadrante que debo dibujar
			}
			else
			{
				switchCase = nVector[currentDepth - 1] + 1;	// para los niveles mas altos, por la estructura del programa, el valor n
			}												// sera igual al cuadrante que debo dibujar menos uno			
			switch (switchCase) // corriminetos correspondientes a cada cuadrante
			{
			case 1:
				break;
			case 2:
				corrX += (width / pow(2, currentDepth)) * 4;
				break;
			case 3:
				corrY += width / pow(2, currentDepth);
				break;
			case 4:
				corrX += (width / pow(2, currentDepth)) * 4;
				corrY += width / pow(2, currentDepth);
				break;
			}
	}
}
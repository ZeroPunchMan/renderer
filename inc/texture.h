#pragma once

#include "math3d.h"

#define COLOR_RGB 0x03
#define COLOR_RGBA 0x04

class Texture
{
public:
	Texture();
	Texture(const char* path);
	~Texture();

	MyColor GetPixel(double u, double v);
	void GenMipMap();
private:
	
	UCHAR *origin = nullptr;
	int width, height, pixelSize;
};

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

	MyMath::Color GetPixel(double u, double v);
	void GetPixel(double u, double v, MyMath::Color *out);
	void GenMipMap();
private:

	UCHAR *origin = nullptr;
	int width, height, pixelSize;
};

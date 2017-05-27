#include "stdafx.h"

#include "texture.h"
#include "stb_image.h"

Texture::Texture() {

}

Texture::Texture(const char* path) {
	//stbi_image_free
	int n;
	origin = stbi_load(path, &width, &height, &n, 0);
	if (origin)
	{
		if (n == 4) {
			this->pixelSize = COLOR_RGBA;
		}
		else if (n == 3) {
			this->pixelSize = COLOR_RGB;
		}
		else {
			MyLog("%s style unknown", path);
		}
	}
	else
	{
		MyLog(_T("Failed"));
	}
}

Texture::~Texture() {
	stbi_image_free(origin);
}

void Texture::GenMipMap() {

}

MyColor Texture::GetPixel(double u, double v) {
	MyColor color;
	if (u < 0)
		u = 0;
	else if (u > 1)
		u = 1;

	if (v < 0)
		v = 0;
	else if (v > 1)
		v = 1;

	int col = u * (width - 1);
	int row = v * (height - 1);
	int rowSize = width * pixelSize;
	int offset = row * rowSize + col * pixelSize;
	color.r = origin[offset] / 256.0f;
	color.g = origin[offset + 1] / 256.0f;
	color.b = origin[offset + 2] / 256.0f;
	if (pixelSize == COLOR_RGBA)
		color.a = origin[offset + 3] / 256.0f;
	else
		color.a = 1.0f;
	return color;
}


void Texture::GetPixel(double u, double v, MyColor *out) {
	if (u < 0)
		u = 0;
	else if (u > 1)
		u = 1;

	if (v < 0)
		v = 0;
	else if (v > 1)
		v = 1;

	int col = u * (width - 1);
	int row = v * (height - 1);
	int rowSize = width * pixelSize;
	int offset = row * rowSize + col * pixelSize;
	out->r = origin[offset] / 256.0f;
	out->g = origin[offset + 1] / 256.0f;
	out->b = origin[offset + 2] / 256.0f;
	if (pixelSize == COLOR_RGBA)
		out->a = origin[offset + 3] / 256.0f;
	else
		out->a = 1.0f;
}

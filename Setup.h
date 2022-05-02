#ifndef SETUP_H
#define SETUP_H

#include <iostream>
#include <vector>
using namespace std;

struct Coord
{
	int x;
	int y;

	Coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Color
{
	int r;
	int g;
	int b;
	int a;

	Color(int r, int g, int b, int a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

struct Pixel
{
	Coord* coord;
	Color* color;

	int mappedHere;

	Pixel(Coord* coord, Color* color)
	{
		mappedHere = 0;
		this->coord = coord;
		this->color = color;
	}
};

struct Image
{
	unsigned int width;
	unsigned int height;

	std::vector<unsigned char> raw;
	std::vector<Pixel*> pixels;

	Image(unsigned int width, unsigned int height, std::vector<unsigned char> raw, std::vector<Pixel*> pixels)
	{
		this->width = width;
		this->height = height;
		this->raw = raw;
		this->pixels = pixels;
	}
};

#endif
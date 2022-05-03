// Pixapper.cpp : Defines the entry point for the application.
//

#include "Pixapper.h"
#include "Setup.h"
#include "external/lodepng/lodepng.h"
#include <iostream>
#include <filesystem>

using namespace std;

Image* DecodePNG(string filename)
{
	std::vector<unsigned char> raw;
	unsigned int width, height;

	unsigned error = lodepng::decode(raw, width, height, filename);

	if (error)
	{
		std::cout << "Error decoding file: " << error << ": " << lodepng_error_text(error) << std::endl;
	}
	else
	{
		std::vector<Pixel*> pixels;

		for (int i = 0; i < raw.size(); i += 4)
		{
			int x = (i / 4) % width;
			int y = round((i / 4) / width);

			Coord* coord = new Coord(x, y);

			int r = raw[i];
			int g = raw[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) + 1];
			int b = raw[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) + 2];
			int a = raw[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) + 3];

			Color* color = new Color(r, g, b, a);

			Pixel* pixel = new Pixel(coord, color);

			pixels.push_back(pixel);
		}

		return new Image(width, height, raw, pixels);
	}

	return nullptr;
}

void MapPNG(Image* source, int cols, int rows, Image* map, string copyPath)
{
	std::vector<unsigned char> newRaw;

	int pixelsMapped = 0;			// How many pixels have we mapped in total?
	int pixelPerMap = cols * rows;	// Maximum source pixels to map to each map pixel.

	int c = source->width / cols;	// Pixels per column.
	int r = source->height / rows;	// Pixels per row.

	int pixPerCell = c * r;			// Pixels per cell of animation.

	for (int i = 0; i < source->pixels.size(); i++)
	{
		pixelsMapped++;
		Pixel* s = source->pixels[i];

		int cellX = floor((s->coord->x - 1) / c) + 1;
		int cellY = floor((s->coord->y - 1) / r);

		int cell = cellX + (cellY * cols);

		if (s->color->r == 0 &&
			s->color->g == 0 &&
			s->color->b == 0 &&
			s->color->a == 0)
		{
			newRaw.push_back(0);
			newRaw.push_back(0);
			newRaw.push_back(0);
			newRaw.push_back(0);
		}
		else
		{
			for (int j = 0; j < map->pixels.size(); j++)
			{
				Pixel* m = map->pixels[j];

				if (m->mappedHere < cell)
				{
					if (s->color->r == m->color->r &&
						s->color->g == m->color->g &&
						s->color->b == m->color->b &&
						s->color->a == m->color->a)
					{
						// std::cout << std::to_string(m->coord->x) + " / " + std::to_string(m->coord->y) + "\n";

						m->mappedHere++;
						newRaw.push_back(m->coord->x);
						newRaw.push_back(-(m->coord->y + 1) + map->height);
						newRaw.push_back(0);
						newRaw.push_back(255);
						break;
					}
				}
			}
		}
	}

	unsigned error = lodepng::encode(copyPath, newRaw, source->width, source->height);

	if (error)
	{
		std::cout << "Error encoding file: " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	return;
}

inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main()
{
	string sourcePath;
	string mapPath;

	int sourceColumns;
	int sourceRows;

	while (true)
	{
		cout << "Please enter the file path for your source file:\n";
		cin >> sourcePath;
		cout << "You entered: " + sourcePath + "\n";
		cout << "Is this correct?\n";
		cout << "Y / n\n";
		string resp;
		cin >> resp;

		if (resp == "n")
		{
			// Cycle Again
		}
		else if (resp== "Y")
		{
			break;
		}
	}

	while (true)
	{
		cout << "---------------------------------------------------------------------------------------------\n";
		cout << "Please enter the the number of columns followed by the number of rows your source sprite has:\n";
		cout << "Columns: ";
		cin >> sourceColumns;
		cout << "Rows: ";
		cin >> sourceRows;
		cout << "You entered: " + std::to_string(sourceColumns) + " columns & " + std::to_string(sourceRows) + " rows.\n";
		cout << "Is this correct?\n";
		cout << "Y / n\n";
		string resp;
		cin >> resp;

		if (resp == "n")
		{
			// Cycle Again
		}
		else if (resp == "Y")
		{
			break;
		}
	}

	while (true)
	{
		cout << "---------------------------------------------\n";
		cout << "Please enter the file path for your map file:\n";
		cin >> mapPath;
		cout << "You entered: " + mapPath + "\n";
		cout << "Is this correct?\n";
		cout << "Y / n\n";
		string resp;
		cin >> resp;

		if (resp == "n")
		{
			// Cycle Again
		}
		else if (resp == "Y")
		{
			break;
		}
	}

	// return 0;

	// Thank you to Lode Vandevenne for providing Lodepng, without which I'd surely flounder.
	// I'm providing Pixapper under the MIT license in part due to its quite niche use.
	// It might be useful for others, but I'm designing it particularly for my own game,
	// The Moonlight Tongue, which is a metroidvania that uses texture mapping to apply
	// visual effects to animations and spice up certain sprites.

	// std::vector<unsigned char> image = DecodePNG("bababooey");

	Image* source = DecodePNG(sourcePath);
	Image* map = DecodePNG(mapPath);

	if (source == nullptr)
	{
		std::cout << "The source png could not be properly loaded.\n";
		return 1;
	}
	else if (map == nullptr)
	{
		std::cout << "The map png could not be properly loaded.\n";
		return 1;
	}

	// Okay, so we've loaded our source and our map into some structs....
	// Now, we want to go through each pixel on the source and find its
	// corresponding pixel on the map, but we'd prefer not to reuse the
	// same pixel on the map for multiple pixels on the source,
	// unless of course we're working with animations, in which case we want
	// to know how many frames there are in the animation so that we can say:
	// X amount of pixels need to have been checked before we can assign another
	// source pixel to the same map pixel. This'll hopefully also ensure that
	// the "same pixel" in one frame is mapped to the same pixel as in
	// the other frames.

	/*for (int i = 0; i < source->pixels.size(); i++)
	{
		std::cout << std::to_string(source->pixels[i]->color->r) + "\n";
	}*/

	std::cout << "Your mapped file has been saved in the -copy.png next to your source file.\n";

	string copyOutput;
	for (int i = 0; i < sourcePath.length() - 4; i++)
	{
		copyOutput += sourcePath[i];
	}
	copyOutput += "-copy.png";

	if (!exists(copyOutput))
	{
		std::filesystem::copy(sourcePath, copyOutput);
	}

	MapPNG(source, sourceColumns, sourceRows, map, copyOutput);

	return 0;
}

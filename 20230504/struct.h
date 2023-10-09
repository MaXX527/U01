#pragma once

#include <wx/string.h>
#include <Magick++.h>

/*
img->compressType(Magick::CompressionType::Group4Compression - BW JPEG Magick::CompressionType::LZWCompression);
img->depth(4);
//img->autoGamma();
img->autoLevel();
img->normalize();
img->strip(); 
*/

struct TIFparam
{
	int rowNumber;
	std::string filename;
	std::string newfilename;
	//Magick::CompressionType compressType;
	size_t depth;
	//bool autoGamma;
	//bool autoLevel;
	//bool normalize;
	//bool strip;
	float rescale;
	int resolution;
	bool convertToGray;
	int exit_code;
};

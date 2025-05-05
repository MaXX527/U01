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
	int rowNumber = 0;
	std::string type = "";
	std::string filename = "";
	std::string newfilename = "";
	std::string pdffilename = "";
	std::string pdfnewfilename = "";
	//Magick::CompressionType compressType;
	size_t depth = 0;
	//bool autoGamma;
	//bool autoLevel;
	//bool normalize;
	//bool strip;
	float rescale = 0.0f;
	int resolution = 0;
	bool convertToGray = false;
	int quality = 0;
	int exit_code = 0;
	int percent = 0;
};

struct MyUserData : wxObject
{
	void* data;
};

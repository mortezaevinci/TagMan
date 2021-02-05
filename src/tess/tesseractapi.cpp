// baseapitester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#define _(x) (x)

using namespace std;




string callOCR(string address)
{
	//if ((argc == 2 && strcmp(argv[1], "-v") == 0) ||
	//	(argc == 2 && strcmp(argv[1], "--version") == 0)) {
	//		fprintf(stderr, "tesseract %s\n", tesseract::TessBaseAPI::Version());
	//		exit(0);
	//}
	// Make the order of args a bit more forgiving than it used to be.
	const char* lang = "eng";
	const char* image = address.c_str();

	const char* output = address.c_str() ;
	tesseract::PageSegMode pagesegmode = tesseract::PSM_AUTO_OSD;


	//int arg = 1;
	//while (arg < argc && (output == NULL || argv[arg][0] == '-')) {
	//	if (strcmp(argv[arg], "-l") == 0 && arg + 1 < argc) {
	//		lang = argv[arg + 1];
	//		++arg;
	//	} else if (strcmp(argv[arg], "-psm") == 0 && arg + 1 < argc) {
	//		pagesegmode = static_cast<tesseract::PageSegMode>(atoi(argv[arg + 1]));
	//		++arg;
	//	} else if (image == NULL) {
	//		image = argv[arg];
	//	} else if (output == NULL) {
	//		output = argv[arg];
	//	}
	//	++arg;
	//}



	tesseract::TessBaseAPI  api;

	api.SetOutputName(output);


	//api.SetVariable("load_punc_dawg",0);
	api.Init("c:", lang, tesseract::OEM_DEFAULT);
			api.SetVariable("tessedit_char_whitelist", "0123456789qwertyuioplkjhgfdsazxcvbnm$ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	// We have 2 possible sources of pagesegmode: a config file and
	// the command line. For backwards compatability reasons, the
	// default in tesseract is tesseract::PSM_SINGLE_BLOCK, but the
	// default for this program is tesseract::PSM_AUTO. We will let
	// the config file take priority, so the command-line default
	// can take priority over the tesseract default, so we use the
	// value from the command line only if the retrieved mode
	// is still tesseract::PSM_SINGLE_BLOCK, indicating no change
	// in any config file. Therefore the only way to force
	// tesseract::PSM_SINGLE_BLOCK is from the command line.
	// It would be simpler if we could set the value before Init,
	// but that doesn't work.
	if (api.GetPageSegMode() == tesseract::PSM_SINGLE_BLOCK)
		api.SetPageSegMode(pagesegmode);
	printf("Tesseract Open Source OCR Engine v%s with Leptonica\n",
		tesseract::TessBaseAPI::Version());


	FILE* fin = fopen(image, "rb");
	if (fin == NULL) {
		printf("Cannot open input file: %s\n", image);
		exit(2);
	}
	fclose(fin);

	PIX   *pixs;
	if ((pixs = pixRead(image)) == NULL) {
		printf("Unsupported image type.\n");
		exit(3);
	}
	pixDestroy(&pixs);

	STRING text_out;
	if (!api.ProcessPages(image, NULL, 0, &text_out)) {
		printf("Error during processing.\n");
	}
	bool output_hocr = false;
	api.GetBoolVariable("tessedit_create_hocr", &output_hocr);
	bool output_box = false;
	api.GetBoolVariable("tessedit_create_boxfile", &output_box);
	STRING outfile = output;
	outfile += output_hocr ? ".html" : output_box ? ".box" : ".txt";
	/*
	FILE* fout = fopen(outfile.string(), "wb");
	if (fout == NULL) {
		printf("Cannot create output file %s\n", outfile.string());
		exit(1);
	}
	fwrite(text_out.string(), 1, text_out.length(), fout);
	fclose(fout);
	*/
	string strout;
	strout=text_out.string();


	return strout;

}

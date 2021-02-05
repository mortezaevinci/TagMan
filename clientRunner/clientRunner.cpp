// clientRunner.cpp : Defines the entry point for the console application.
//

#include "iostream"
#include "string"
#include "windows.h"

using namespace std;


int main(int argc, char* argv[])
{
	if (argc<2)
		return 0;

	string target="q:\\temp\\tagman_image1.jpg"; //should be loaded instead from the config file

	CopyFileA(argv[1],target.c_str(),false);
	cout<<"file copied.";

	system("wcOCR.exe");
	cout<<"client is executed.";
	return 0;
}


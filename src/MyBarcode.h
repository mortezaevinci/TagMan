#ifndef _myCORE_
#include "myCORE.h"
#endif



using namespace cv;
using namespace std;

void findBarlines(InputArray _image, imageLabel *imageLabel0,Size LF_size,double LF_sigma,int CB_size, double* BAR_rate,double CF_thresh,
	minMax CONTOUR_rate,double CONTOUR_fillrate,double BARLINE_se_size);

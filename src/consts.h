#ifndef _myCORE_
#include "myCORE.h"
#endif

#ifndef _myconsts_
#define _myconsts_
#endif

double Hough_bAcc=1, Hough_dAcc=CV_PI/180, Hough_thresh=50, Hough_minLength=100, Hough_maxGap=50;
int minmaxRadiousBase[]={5,9,11};
int Binarization_rangeThresh=25;
int Barcodelines_averageGap=5;
Size Barcodelines_LF_size (5,9);
double Barcodelines_LF_sigma=5;
int Barcodelines_CB_size=10;
double Barcodelines_BAR_rate[]={0.2,0.15};
double Barcodelines_CF_thresh=0.75;
minMax Barcodelines_CONTOUR_rate={0.005,0.2};
double Barcodelines_CONTOUR_fillrate=0.5;
double Barcodelines_BARLINE_se_size=10;

int rate_thresh=45;
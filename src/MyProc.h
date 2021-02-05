#ifndef _myCORE_
#include "myCORE.h"
#endif

using namespace cv;

void directionalDifference(InputArray _image,OutputArray _dirDiff,Size LF_size,double LF_sigma,int CB_size);

Point contourCentroid(vector<Point> contour);

void clearBorder(InputArray _src,OutputArray _dst,int bordersize);

void stretchlimitsFromHist(cv::MatND& hist, double* low_found,
                     double* high_found, double low_float, double high_float,
                     unsigned int Sum_hist);

void stretchlimits( const InputArray image, double* low_found,double* high_found, double low_float = 0.01,double high_float = 0.99);
//! adjust image pixel values by linear mapping
void contrastadjust( const InputArray src, OutputArray dst,double low_in = 0, double high_in = 0,double low_out = 0, double high_out = 1 );

int bitsFromDepth( int depth );

void windowThreshold(InputArray image,OutputArray  result, OutputArray criterion, int ThInTh, int windowSize,int sigma);

void MinMax(InputArray _image,int MinMax[2]);

void MeanLowHigh(InputArray _image, double Mean[2], double T);

void Otsu( InputArray image, OutputArray dst);

void windowAdjust(InputArray _image, OutputArray _result,OutputArray _criterion,int radius,int rangeThresh);

void dirEdge(InputArray src, OutputArray des,char dir=0, int ddepth=-1,double delta=0, Point anchor=Point(-1,-1));

void blackHatThreshold(Mat& image,Mat&  result, Mat& criterion, int ThInTh=-1, int windowSize=25,int sigma=1, int morph_size=9, int medianSize=1);

void adjustedBinary1(InputArray _image, OutputArray _result,OutputArray _criterion,int radius,int rangeThresh);

void adjustedBinary2(InputArray _image, OutputArray _result,OutputArray _criterion,int* radiusSet,int rangeThresh);
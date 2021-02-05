#ifndef _myCORE_
#define _myCORE_
#endif

#define _DEBUG_

#ifdef _DEBUG_
#define coutdd(x)  cout << x;
#define coutdl(x) cout <<x<<endl;
#define imageshow(x,y) imshow(x,y);
#define imshowdd(x,y) imshow(x,y);
#define imwritedd(x,y) imwrite(x,y);
#else
#define coutdd(x)  
#define coutdl(x)
#define imageshow(x,y)
#define imshowdd(x,y)
#define imwritedd(x,y)

#endif

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <limits>


using namespace cv;
using namespace std;

//barlines
struct minMax
{
	double min;
	double max;
};

struct seedLabel
{
	Point seed;
	unsigned int validity;

};

struct imageLabel
{
    vector<seedLabel> seedLabels;
	Mat image;
};

//ransac
struct RANSACparams
{
	int sample_size;
	int max_samples;
	double max_risk;
	double inlierThresh;
};

struct precalcsForLines
{
	Mat distanceImage;
	float distanceThresh;
};

template <class sample>
void xyminmax(vector<sample> points,int* xmin,int* xmax,int* ymin, int* ymax)
{

	*xmin=numeric_limits<int>::min();
	*xmax=numeric_limits<int>::max();
	*ymin=numeric_limits<int>::min();
	*ymax=numeric_limits<int>::max();

	for(int ii=0;ii<points.size();ii++)
	{
		if (points[ii].x>*xmax)
			*xmax=points[ii].x;
		if (points[ii].x<*xmin)
			*xmin=points[ii].x;
		if (points[ii].y>*ymax)
			*ymax=points[ii].y;
		if (points[ii].y<*ymin)
			*ymin=points[ii].y;
	}
};

#include "opencv2/imgproc/imgproc.hpp"

#include "myCORE.h"
#include "MyProc.h"
#include "myRANSAC.h"

#include <iostream>

#define randf ((float)(rand()%100)/100);
#define addpoint(x,y,v) plotImage.at<unsigned char>(y+400,x+400)=v;

using namespace cv;
using namespace std;








void RANSAClineTest()
{
	int ninliers=30;
	int noutliers=30;
	float sigma=5;

	vector<Point2f> samples;
	float xi,yi,xo,yo,noise;

	srand(time(NULL));

		Mat plotImage;
	plotImage=Mat::zeros(800,800,CV_8U);
	float rand0=randf;
	//generate inliers
	coutdd(endl<<"inliers: "<<endl);
	for(int ii=0;ii<ninliers;ii++)
	{
		xi=randf; noise=randf;
		//coutdd("randftest:"<<xi);
		xi=(20*xi-10)*20;
		yi=(rand0*xi+2*rand0-1+sigma*noise);
		Point2f point0 ((int)xi,(int)yi);
		coutdd(point0);
		samples.push_back(point0);
		addpoint(xi,yi,255);
	}

	coutdd(endl<<"outliers: "<<endl);
	for(int ii=0;ii<noutliers;ii++)
	{
		xo=randf; xo=(3*xo-1.5)*20;
		yo=randf; yo=(3*yo-1.5)*20;
		Point2f point0 ((int)xo,(int)yo);
		coutdd(point0);
		samples.push_back(point0);
		addpoint(xo,yo,100);
	}


	RANSACparams paramstest0={2,10,.0001,2};

	 //RANSAC test
			vector<Point2f> inliers;
			int precalc0;
			 Mat* foundLines=new Mat[paramstest0.max_samples];
			 float* lineMeasures=new float[paramstest0.max_samples];
			 //RANSAC <Point,Mat,Mat>(points0,RANSACline,inliers,distanceImage, params0,&myFitLine, &evaluateLine,&evaluateRANSAC);

			 myRANSAC<Point2f,OutputArray,Mat,int>(samples,foundLines,lineMeasures,inliers,precalc0,paramstest0,&myFitLine,&evaluateLine,&evaluateRANSACforLines);

			 //find best line
			 float bestMeasure=-1000;
			 int bestInd=-1;
			 for(int ii=0;ii<sizeof(lineMeasures)/sizeof(float);ii++)
				 if (lineMeasures[ii]>bestMeasure)
				 {
					 bestInd=ii;
					 bestMeasure=lineMeasures[ii];
				 }
				 coutdd(endl<<"a:"<<foundLines[bestInd].at<float>(1)*200<<endl<<"c:"<<foundLines[bestInd].at<float>(3)<<endl<<"b:"<<foundLines[bestInd].at<float>(2)<<endl);
				 coutdd("y:"<<(foundLines[bestInd].at<float>(1)*200+foundLines[bestInd].at<float>(3))/foundLines[bestInd].at<float>(2)<<endl);

				 	 cvtColor(plotImage,plotImage,CV_GRAY2BGR);
				 	 line(plotImage,Point(0+400,400-(foundLines[bestInd].at<float>(0)*0+foundLines[bestInd].at<float>(2))/foundLines[bestInd].at<float>(1)),
						 Point(200+400,400-(foundLines[bestInd].at<float>(0)*200+foundLines[bestInd].at<float>(2))/foundLines[bestInd].at<float>(1)),
						 Scalar(0,0,255),1,CV_AA);

				 	// line(plotImage,Point(1,1), Point(300,300), Scalar(0,0,255),3,CV_AA);



				 coutdd("best ransac line: "<<foundLines[bestInd]<<endl);

		imageshow("",plotImage);
}
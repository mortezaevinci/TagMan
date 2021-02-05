#ifndef _myCORE_
#include "myCORE.h"
#endif

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <iostream>
#include <time.h>       /* time */

using namespace cv;
using namespace std;

template <class sample>
int myFitLine(vector<sample> points, OutputArray _line)
{
Mat line=_line.getMat();

	if (points.size()<2)
		//no line can be found
		return 1; //error

	if (points.size()==2)
	{
		float data[2][3]={{points[0].x,points[0].y,1},{points[1].x,points[1].y,1}};

		Mat v[2];

		for(int i=0;i<2;i++)
			v[i]=Mat(3,1,CV_32FC1,data[i]);
		
		line=(v[0].cross(v[1]));
		normalize(line,line);

		line.copyTo(_line);

		return 0;
	}

	Scalar mean_;
    //reduce(points, mean_, 1,CV_REDUCE_AVG, 1);
	mean_=mean(points);
    Point2f mean0(mean_[0], mean_[1]); 

	
	float sxx,sxy,syy;
	sxx=0; syy=0; sxy=0;
	for(int vv=0;vv<points.size();vv++)
	{
		//points[vv]=points[vv]-mean;
		sxx+=((float)points[vv].x-mean0.x)*((float)points[vv].x-mean0.x);
		syy+=((float)points[vv].y-mean0.y)*((float)points[vv].y-mean0.y);
		sxy+=((float)points[vv].y-mean0.y)*((float)points[vv].x-mean0.x);
	}
	
	//create the scatter matrix
	Mat SC;
	SC=Mat::zeros(2,2,CV_32FC1);
	float data[2][2]={{sxx,sxy},{sxy,syy}};
	SC=Mat(2,2,CV_32FC1,data);

	Mat eigD,eigV;

	eigen(SC,eigD,eigV);
	

	if ( (eigD.at<float>(0)<0)||(eigD.at<float>(1)<0))//||(eigD.at<float>(0)>eigD.at<float>(1)))
		return 1; //error

	float a,b;

	if (eigD.at<float>(0)>eigD.at<float>(1))
	{
	a=eigV.at<float>(1,0);
	b=eigV.at<float>(1,1);
	}
	else
	{
	a=eigV.at<float>(0,0);
	b=eigV.at<float>(0,1);
	}
	float linedata[3]={a,b,-(a*mean0.x+b*mean0.y)};

	line=Mat(3,1,CV_32FC1,linedata);

	line.copyTo(_line);
	
return 0;
};

template <class sample>
void evaluateLine(vector<sample> points,Mat line, float *d)
{
	float a=line.at<float>(0);
	float b=line.at<float>(1);
	float c=line.at<float>(2);


	for(int ii=0;ii<points.size();ii++)
       d[ii]=abs(a*points[ii].x+b*points[ii].y+c)/sqrt(a*a+b*b);


};

template <class sample>
void evaluateRANSACforLines(vector<sample> inliers,precalcsForLines precalc,float *measure)
{

	int xmin,xmax,ymin,ymax;
	float distMax;
	xyminmax(inliers,&xmin,&xmax,&ymin,&ymax);

	distMax=(xmin-xmax)*(xmin-xmax)+(ymin-ymax)*(ymin-ymax);

	//calculate entropy
	Scalar mean_=mean(inliers);
	//Scalar sum_=sum(inliers);

	float Ex=mean_[0];
	float Ey=mean_[1];

	double entropy=0;

	float distk=0;

	for(int ii=0;ii<inliers.size();ii++)
	{
		float dx,dy;
		double p;
		float disttemp=precalc.distanceImage.at<float>(inliers[ii].y,inliers[ii].x);

		if ((0<disttemp)&&(disttemp<precalc.distanceThresh))
			distk+=1/disttemp;

		dx=(float)inliers[ii].x-Ex;
		dy=(float)inliers[ii].y-Ey;
		p=sqrt(dx*dx+dy*dy)/distMax;
		if (p!=0)
			entropy+=-p*log(p)/log((double)2);

		coutdd("p:"<<p<<"  ento: "<<-p*log(p)/log((float)2)<<endl);
	}

	coutdd("distk:"<<distk<<"  entropy:"<<entropy<<endl);

	*measure=distk*entropy;
};

template <class sample>
void evaluateRANSACforLines(vector<sample> inliers,int precalc,float *measure)
{

	int xmin,xmax,ymin,ymax;
	float distMax;
	xyminmax<sample>(inliers,&xmin,&xmax,&ymin,&ymax);

	distMax=(xmin-xmax)*(xmin-xmax)+(ymin-ymax)*(ymin-ymax);


	
	*measure=inliers.size()*distMax;
};

void RANSAClineTest();

template <class sample,class modelout, class modelin, class precalcs> //precalcs can be the distance image
int myRANSAC(vector<sample> samples,modelin* models,float* measures,vector<sample> fitted,precalcs data, RANSACparams params,int (*fitter)(vector<sample>,modelout),
	void (*modelevaluator)(vector<sample>,modelin,float*),void (*RANSACevaluator)(vector<sample>,precalcs,float*))
{
	coutdd("ransac!"<<endl);
		if (samples.size()<params.sample_size)
		return 1; //error


		//this two have to be done out of here, so they can be neatly cleared up at the end (delete[])
	

	float *d=new float[samples.size()];
	bool *selected=new bool[samples.size()];

		vector<int> selInd;
	vector<sample> selSamples;

			 vector<int> inlierInd;
		 vector<sample>inlierSamples;

		 			srand(time(NULL));

	for(int nsample=0;nsample<params.max_samples;nsample++)
	{
		coutdd(endl<<"generate selection"<<endl);


	//clear
	for(int ii=0;ii<samples.size();ii++) selected[ii]=false;
	selSamples.clear();
	selInd.clear();
	inlierInd.clear();
	inlierSamples.clear();

	//generate a random selection

	int cnt=0;


	while(cnt<params.sample_size)
	{
		int itemp;
		
		
		itemp=rand()%samples.size();
		if (~selected[itemp])
		{
			cnt++;
			selected[itemp]=true;
			selInd.push_back(itemp);
			selSamples.push_back(samples[itemp]);
			//show
			coutdd("itemp:"<<itemp<<" samples[itemp]:"<< samples[itemp]<<endl);
		}
	}

	coutdd(endl<<"fit to model"<<endl);

		//fit to the model


		 if ((*fitter)(selSamples,models[nsample]))
			 coutdd("fitted"<<endl);

		 coutdd(models[nsample]);

		  //evaluate the model for all samples
		 (*modelevaluator)(samples,models[nsample],d);


		 //coutdd(d);



		 coutdd(endl<<"find inliers"<<endl);

		 //find all the inliers
		 for(int ii=0;ii<samples.size();ii++)
			 if (d[ii]<params.inlierThresh)
			 {
				 inlierInd.push_back(ii);
				 inlierSamples.push_back(samples[ii]);

				 coutdd(samples[ii]);
			 }

			 coutdd(endl<<"calc measure"<<endl);

		//calculate RANSAC measure

		(*RANSACevaluator)(inlierSamples,data,&measures[nsample]);

		coutdd("measure:"<<measures[nsample]<<endl);

		//TODO: check the risk, and if it is reliable, stop finding another line

	}//end of nsample


	//clean up
	delete [] selected;
	delete [] d;

	return 0;
}
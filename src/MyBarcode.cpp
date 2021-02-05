#include "opencv2/highgui/highgui.hpp"

#include "myCORE.h"
#include "MyProc.h"
#include "MyBarcode.h"

#include <iostream>

using namespace cv;
using namespace std;

//source should be already gray and binarized

void findBarlines(InputArray _image, imageLabel *imageLabel0,Size LF_size,double LF_sigma,int CB_size, double* BAR_rate,double CF_thresh,
	minMax CONTOUR_rate,double CONTOUR_fillrate,double BARLINE_se_size)
{
	Mat image = _image.getMat();

	Mat img2;

	Mat invopenimg;




		Mat element = getStructuringElement(MORPH_RECT  , Size( BARLINE_se_size, 1 ), Point( -1,-1) );

		int area=image.cols*image.rows;


	Mat dirDiff;
	//dirDiff.zeros(image.size(),image.type());

	double temp,MaxDiff;
	Point temp2,temp3;
	double levelCf;

		Mat Cf;
		Cf.zeros(image.size(),image.type());
	
		int labelCount[2]={0,0};
	

	//do the procedure two times with 0 and 90 degrees rotation
	for(int rot=0;rot<2;rot++)
	{
		if (rot==1)
		{
		//rotation
			transpose(image,img2);
			flip(img2,img2,1);
		}
		else
			img2=image.clone();
	  
			//invopenimg.create(img2.size(), img2.type());
			invopenimg=img2.clone();

		//taking gradient
		 GaussianBlur( img2, img2, Size(3,3), 0, 0, BORDER_DEFAULT );

		 //calculating directional Difference using 2D gradient
		 //directionalDifference(img2,dirDiff,LF_size,LF_sigma,CB_size);
		 	int ddepth = CV_16S;
			int scale = 1;
    int delta = 0;
	

	 	Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
		 /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( img2, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

   /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( img2, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

      dirDiff=abs_grad_x-abs_grad_y;

     	  //big low pass filter
		 GaussianBlur( dirDiff, dirDiff, LF_size, LF_sigma, LF_sigma, BORDER_DEFAULT);

		//clear borders
		 clearBorder(dirDiff,dirDiff,CB_size);

		  //average over barcodes with minimum size
		 int bx=(int)image.cols*BAR_rate[0];
		 int by=(int)image.rows*BAR_rate[1];


		 blur( dirDiff, dirDiff, Size (bx,by), Point(-1,-1));

		 //threshold the dirDiff to find barline spots
		 minMaxLoc(dirDiff,&temp,&MaxDiff,&temp2,&temp3);

		 levelCf=MaxDiff*CF_thresh;

		 threshold(dirDiff,Cf,levelCf,255,cv::THRESH_BINARY);


		 //opned and inversed gray image

	//morphologyEx(img2,invopenimg,MORPH_OPEN,element,Point(-1,-1),BORDER_DEFAULT );
		erode(img2,invopenimg,element);
		dilate(invopenimg,invopenimg,element);

	    //invopenimg=~invopenimg;

		threshold(invopenimg,invopenimg,150,255,THRESH_BINARY_INV);


		 //find contuors in Cf
		 vector<vector<Point>> contoursCf;
		 findContours(Cf, contoursCf, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);
		


		 //find contours in invopened that intersect with big contuors in Cf
		 int minarea=CONTOUR_rate.min*area;
		 int maxarea=CONTOUR_rate.max*area;


		 	 Scalar colorcc = Scalar( 0,0,255);
			 		 	 Scalar colorii = Scalar( 255,255,255);
		   Mat drawing = Mat::zeros( Cf.size(), CV_8UC3 );




		 Rect Rcc,Rii,Rintersect;


		 for(int cc=0;cc<contoursCf.size();cc++)
			 if (contourArea(contoursCf[cc])>minarea && contourArea(contoursCf[cc])<maxarea)
					 { 	//if intersecting, add the contoursi to collection

					 Point Centroid=contourCentroid(contoursCf[cc]);


					//include centroid and all contourpoints in invopned, and then connect altogether
					 for(int pp=0;pp<contoursCf[cc].size();pp++)
					 {
						 Point point0=contoursCf[cc][pp];

						 point0.x=point0.x/2+Centroid.x/2;
						 point0.y=point0.y/2+Centroid.y/2;

						 if (invopenimg.at<unsigned char>(point0.y, point0.x)==255) //a unused area is detected
						 {
							 //grow the region
							 labelCount[rot]++;

							 int filledarea;
							 Rect BoundingRect0;
							 filledarea=floodFill(invopenimg,point0,labelCount[rot],&BoundingRect0);

							 if (filledarea>minarea && filledarea<maxarea && (filledarea>BoundingRect0.area()*CONTOUR_fillrate))
							 {
							 //fix the seedLabel0
							 seedLabel ltemp;
							 ltemp.validity=1;
							 ltemp.seed=point0;
							 //seedLabel0[rot][labelCount[rot]].validity=1;
							 //seedLabel0[rot][labelCount[rot]].seed=point0;
							 imageLabel0[rot].seedLabels.push_back(ltemp);
							 imageLabel0[rot].seedLabels[labelCount[rot]].seed=point0;
							 imageLabel0[rot].seedLabels[labelCount[rot]].validity=1;
							 }
							 else
								 labelCount[rot]--;
						 }
						 else if (invopenimg.at<unsigned char>(point0.y, point0.x)>0 && invopenimg.at<unsigned char>(point0.y, point0.x)<=labelCount[rot]) 
						 {
							
								unsigned int itemp=(unsigned int)invopenimg.at<unsigned char>(point0.y, point0.x);
						 		imageLabel0[rot].seedLabels[itemp].validity+=1;
						 }


					 }


			

				 }

			 //to show the labeled sections

			 /*
			 		 		 cout<<"no of labels"<<labelCount[rot]<<endl;
			 for(int ll=1;ll<=labelCount[rot];ll++)
			 {
				 cout<<"point "<<imageLabel0[rot].seedLabels[ll].seed<<endl;
				 cout<<"validity of area"<<imageLabel0[rot].seedLabels[ll].validity<<endl;

			 	imshow("",invopenimg==ll);
				    waitKey(0);
			 }
			 */
	
			//	 imshow("",drawing);

		if (rot==1)
		{
		//rotation
			flip(invopenimg,imageLabel0[rot].image,1);
			transpose(imageLabel0[rot].image,imageLabel0[rot].image);
			
		}
		else
			imageLabel0[rot].image=invopenimg.clone();

   	invopenimg.release();
	}


	Cf.release();
	dirDiff.release();
};
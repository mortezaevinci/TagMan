#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <iostream>
#include "ImageUtils.h"
#include "myCORE.h"
#include "MyProc.h"
#include "MyBarcode.h"
#include "myRANSAC.h"
#include <string.h>
#include "consts.h"



using namespace cv;
using namespace std;

const char* keys =
{
    "{1| |1.jpg|input image name}"
};




//TODO:fix all these based on image size, or keep the image size constant to some big value



static void help()
{
    coutdd("\TagMan!\n"
           "Call:\n"
           " [image_name -- Default is 1.jpg]\n\n");
}




int main( int argc, const char** argv )
{
    help();

    CommandLineParser parser(argc, argv, keys);
    string filename = parser.get<string>("1");

   Mat image;

    image = imread(filename, 1);
    if(image.empty())
    {
        printf("Cannot read image file: %s\n", filename.c_str());
        help();
        return -1;
    } //init _end

//	imshow("1",image);

	Mat grayS;
	cvtColor(image, grayS, CV_BGR2GRAY);

	       double resizerate=2; //TODO:use a variable resizerate, and instead make the image size constant

	//  {
	
	//directional edge detection

	Mat EHor;
	EHor.create(grayS.size(), grayS.type());
	 Mat EVer;
	 EVer.create(grayS.size(), grayS.type());

     dirEdge(grayS, EHor);
	
	 dirEdge(grayS,EVer,1);

     threshold(EVer,EVer,128,255,CV_THRESH_BINARY);

	 threshold(EHor,EHor,128,255,CV_THRESH_BINARY);

	 //calculating hough transform lines vertical
	 Mat cdstV;
	 cvtColor(grayS,cdstV,CV_GRAY2BGR);

	 vector<Vec4i> linesV;
	 HoughLinesP(EVer, linesV,Hough_bAcc, Hough_dAcc, Hough_thresh, Hough_minLength, Hough_maxGap);

	 for(size_t i=0;i<min<int>(linesV.size(),6);i++)
	 {
		 Vec4i l=linesV[i];
		 line(cdstV,Point(l[0],l[1]),Point(l[2],l[3]), Scalar(0,0,255),3,CV_AA);
	 }

	 imageshow("detected lines Ver",cdstV);
	 imwrite("result/Vertical_lines.jpg",cdstV);

	  //calculating hough transform lines vertical
	 Mat cdstH;
	 cvtColor(grayS,cdstH,CV_GRAY2BGR);

	 vector<Vec4i> linesH;
	 HoughLinesP(EHor, linesH,1,CV_PI/180, 50,100,50);

	 for(size_t i=0;i<min<int>(linesH.size(),6);i++)
	 {
		 Vec4i l=linesH[i];
		 line(cdstH,Point(l[0],l[1]),Point(l[2],l[3]), Scalar(0,0,255),3,CV_AA);
	 }

	imageshow("detected lines Hor",cdstH);
	 imwrite("result/Horizontal_lines.jpg",cdstH);

	 //} hough lines of general picture _end

//interpolation
	   Size sSize=grayS.size();

  Mat gray;
  Size gSize=Size((int)(sSize.width*resizerate),(int)(sSize.height*resizerate));
//  gray2.create(gSize,grayS.type());
     
 resize(grayS,gray,gSize); //interpolation _end
  
 //contrast adjustment   
	
 	 double lowFound, highFound;
	 double lowFloat=1;
	 double highFloat=99;

     stretchlimits(gray, &lowFound, &highFound, lowFloat/100.0, highFloat/100.0);
     contrastadjust(gray, gray, lowFound, highFound);
    
	 //in larger images, maybe a median would be a nice preprocess
	 //int medianSize=1;
	 //medianBlur(adjusted,adjusted, medianSize);

 
		 Mat adaptiveGrayB,adaptiveCriterionB;
	 //Criterion represents where important information are located.

	 adaptiveGrayB.create(gray.size(),gray.type());
	 adaptiveCriterionB.create(gray.size(),gray.type()); //contrast adjustment _end

 //nonlinear window-based adjusted binarization 2
		 
	 int MinMaxRadiousSet[3];//={(int)(minmaxR*resizerate),(int)(9*resizerate),(int)(11*resizerate)};
	 for(int i=0;i<3;i++)
	  MinMaxRadiousSet[i]=(int)(minmaxRadiousBase[i]*resizerate);

	 adjustedBinary2(gray,adaptiveGrayB,adaptiveCriterionB,MinMaxRadiousSet,Binarization_rangeThresh);

	// namedWindow("adjustedBinary2",1);
	 imageshow("adjustedBinary2",adaptiveGrayB);
	 imwrite("result/adjustedBinary2.jpg",adaptiveGrayB); //nonlinear window-based adjusted binarization 2 _end

	 //calculating hough transform lines on binary
	

	 vector<Vec4i> linesB;

	 for(int jj=5;jj<10;jj=jj+5)
	 {

	 HoughLinesP(255-adaptiveGrayB, linesB,1,CV_PI/360, jj,(int)(50*resizerate-jj/5),2);
	 
	 if (1==0) //don't do anything for now, and let me write the whole code first.
	 for(size_t i=0;i<linesB.size();i++)
	 {
		 Vec4i l=linesB[i];
		 line(adaptiveGrayB,Point(l[0],l[1]),Point(l[2],l[3]), 255,3,CV_AA);
	 }

	 }

	 imageshow("detected lines BW",adaptiveGrayB);
	 imwrite("result/adjustedBinaryNoLines.jpg",adaptiveGrayB); //hough lines of general picture, and filtering out unwanted lines _end


	 //find barlines
	 //in low quality images, barcode is not detectable directly, so we have to fine the barlines instead.

	 imageLabel *barcodelines_imageLabel=new imageLabel[2];

	 //******resizing back to the small size by 1/resizerate might be a good idea
	 
	 findBarlines(adaptiveGrayB,barcodelines_imageLabel,Barcodelines_LF_size,Barcodelines_LF_sigma,Barcodelines_CB_size,Barcodelines_BAR_rate,
		 Barcodelines_CF_thresh,Barcodelines_CONTOUR_rate,Barcodelines_CONTOUR_fillrate,Barcodelines_BARLINE_se_size*resizerate);

	 string ff;//="result/barlines";

	  	for(int rot=0;rot<2;rot++)
			 for(int ll=0;ll<barcodelines_imageLabel[rot].seedLabels.size();ll++)
			 {
				// cout<<"point "<<barcodelines_imageLabel[rot].seedLabels[ll].seed<<endl;
				// cout<<"validity of area"<<barcodelines_imageLabel[rot].seedLabels[ll].validity<<endl;

				 Mat subbar=(((barcodelines_imageLabel[rot].image!=(ll+1))|~adaptiveGrayB)&(barcodelines_imageLabel[rot].image==(ll+1)));
			 	imageshow("",subbar);
				
				
				string ccrot=to_string((long double)rot);
				string ccll=to_string((long double)ll);
				ff="result/barlines_";
				ff.append(string(ccrot));
				ff.append("_");
				ff.append(string(ccll));
				ff.append(".jpg");
				imwrite(ff,subbar);
				//    waitKey(0);
			 }
			 

	 //find barlines_END


			
//fitline example
 vector<Point> points0;
			 points0.push_back(Point(0,1));
			 points0.push_back(Point(1,3));
			 points0.push_back(Point(2,5));
			 points0.push_back(Point(2,25));

			 Mat line0;

			 //fitLine(points0,line0,CV_DIST_L2,0,0.01,0.01);
			 if (~myFitLine(points0,line0))
			  	coutdd("fit line to all:"<<line0<<endl);

			 //evaluate line
			 float *d=new float[points0.size()];
			 evaluateLine(points0,line0,d);
//end of fitline example

			//testing ransac for random numbers
			 RANSAClineTest();

			//TODO:use RANSAC on barcodes (part 1): detect lines

			//find distance image
			Mat distanceImage;
			distanceTransform(~adaptiveGrayB, distanceImage, CV_DIST_L2, 3);

			precalcsForLines precalc0;
			precalc0.distanceImage=distanceImage;
			precalc0.distanceThresh=5;

			 //RANSAC test basic1

			 vector<Point> inliers;
			 distanceImage=adaptiveGrayB.clone();
			 RANSACparams params0={2,5,.0001,3};
			 Mat* foundLines=new Mat[params0.max_samples];
			  float* lineMeasures=new float[params0.max_samples];
			 //RANSAC <Point,Mat,Mat>(points0,RANSACline,inliers,distanceImage, params0,&myFitLine, &evaluateLine,&evaluateRANSAC);

			 myRANSAC<Point,OutputArray,Mat,precalcsForLines>(points0,foundLines,lineMeasures,inliers,precalc0,params0,&myFitLine,&evaluateLine,&evaluateRANSACforLines);

			 //find best line
			 float bestMeasure=-1000;
			 int bestInd=-1;
			 for(int ii=0;ii<sizeof(lineMeasures)/sizeof(float);ii++)
				 if (lineMeasures[ii]>bestMeasure)
				 {
					 bestInd=ii;
					 bestMeasure=lineMeasures[ii];
				 }

				 coutdd("best ransac line: "<<foundLines[bestInd]<<endl);

			//TODO:use RANSAC on barcodes (part 2): detect fundamental matrix for lines

			//TODO:use RANSAC on barcodes (part 3): detect grouped lines

			//TODO:use RANSAC on barcodes (part 4): detect barcodesize


			 //TODO:clean up
			 delete [] d;
			 delete [] foundLines; //TODO:also release mats
			 delete [] lineMeasures;
			 delete [] barcodelines_imageLabel;

	 // Wait for a key stroke; the same function arranges events processing
    waitKey(0);
    return 0;
}

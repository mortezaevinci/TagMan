#include "myCORE.h"
#include "MyProc.h"


using namespace cv;
using namespace std;

void directionalDifference(InputArray _image,OutputArray _dirDiff,Size LF_size,double LF_sigma,int CB_size)
{
		int ddepth = CV_16S;
			int scale = 1;
    int delta = 0;

	Mat img2=_image.getMat();
	Mat dirDiff=_dirDiff.getMat();

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
}

Point contourCentroid(vector<Point> contour)
{

 Moments mu;
 mu = moments( contour, false );

///  Get the mass center
  Point Centroid = Point( (int)mu.m10/mu.m00 , (int)mu.m01/mu.m00 );

  return Centroid;
}

void clearBorder(InputArray _src,OutputArray _dst,int bordersize)
{
	Mat src=_src.getMat();
	Mat dst=_dst.getMat();

	int c=src.cols;
	int r=src.rows;

	//if bordersize is too big, then dst would be zero anyways. This will prevent errors from next part.
		if ((bordersize>c/2) || (bordersize>r/2))
		{
			dst.zeros(src.size(),src.type());
			return;
		}

	//src.copyTo(dst);
		dst=src.clone();

	for (int y=0;y<bordersize;y++)
		for(int x=0;x<c;x++)
			dst.at<unsigned char>(y, x)=0;

	for (int y=r-bordersize;y<r;y++)
		for(int x=0;x<c;x++)
			dst.at<unsigned char>(y, x)=0;

	for (int x=0;x<bordersize;x++)
		for(int y=0;y<r;y++)
			dst.at<unsigned char>(y, x)=0;

	for (int x=c-bordersize;x<c;x++)
		for(int y=0;y<r;y++)
			dst.at<unsigned char>(y, x)=0;
}

void stretchlimitsFromHist(cv::MatND& hist, double* low_found,
                     double* high_found, double low_float, double high_float,
                     unsigned int Sum_hist)
{
    CV_Assert( low_float >= 0 && low_float < 1.0 );
    CV_Assert( low_float < high_float && high_float <= 1.0);

    unsigned int sum;
    unsigned int low_cnt = low_float * Sum_hist;
    sum = 0;
    for( unsigned int i = 0; i < hist.rows; i++ ) {
        if (sum >= low_cnt) {
            *low_found = i;
            break;
        }

        sum += ((float*)hist.data)[i];
    }

    unsigned int high_cnt = (1 - high_float) * Sum_hist;
    sum = 0;
    for( unsigned int i = hist.rows - 1; i >= 0; i-- ) {
        if (sum >= high_cnt) {
            *high_found = i;
            break;
        }

        sum += ((float*)hist.data)[i];
    }
}


int depthToBits( int depth )
{
    if (depth == CV_8U)
        return 8;
    else if (depth == CV_16U)
        return 16;
    else
        return 0;
}




//TODO: handle RGB or force user to do a channel at a time?
void stretchlimits(InputArray _image, double* low_found,
                     double* high_found, double low_float, double high_float )
{
    Mat image = _image.getMat();

    CV_Assert( image.type() == CV_8UC1 || image.type() == CV_16UC1 );

    int nPixelValues = 1 << depthToBits( image.depth() );
    int channels[] = { 0 };
    MatND hist;
    int histSize[] = { nPixelValues };
    float range[] = { 0, nPixelValues };
    const float* ranges[] = { range };
    calcHist( &image, 1, channels, Mat(), hist, 1, histSize, ranges );
    
    stretchlimitsFromHist( hist, low_found, high_found, low_float, high_float, image.rows * image.cols );

    //TODO: scaling to 0..1 here, but should be in stretchlimitsFromHist?
    unsigned int maxVal = (1 << depthToBits( _image.depth() )) - 1;
    *low_found /= maxVal;
    *high_found /= maxVal;
}

//TODO: best to determine output depth from _dst or argument?
void contrastadjust(InputArray _src, OutputArray _dst, double low_in,
                   double high_in, double low_out, double high_out )
{
    CV_Assert( (low_in == 0 || high_in != low_in) && high_out != low_out );

	//not really needed
    if (low_in == 0 && high_in == 0)
        stretchlimits ( _src, &low_in, &high_in );

    double m = (high_out - low_out) / (high_in - low_in);
    double b = high_out - high_in * m;

    Mat src = _src.getMat();
    int depth;
	depth = _src.depth();


    int max_in = (1 << depthToBits( _src.depth() )) - 1;
    int max_out = (1 << depthToBits( _dst.depth() )) - 1;
    // y = a*x*(maxout/maxin) + b*maxout
    m =m* max_out / max_in;
    b =b* max_out;

    src.convertTo( _dst, depth, m, b );
}


void windowThreshold(InputArray _image,OutputArray _result, OutputArray _criterion, int ThInTh, int windowSize,int sigma)
{
	 Mat image = _image.getMat();
     Mat result=_result.getMat();
	 Mat criterion=_criterion.getMat();
	 Size imageSize=image.size();

	 Mat grayThresh;
	 grayThresh.create(image.size(),image.type());

	 Size ksize(windowSize,windowSize);
	
	 GaussianBlur(image,grayThresh,ksize,sigma,sigma);



	 for(int i=0;i<imageSize.width;i++)
		 for(int j=0;j<imageSize.height;j++)
		 {
			 criterion.at<unsigned char>(j,i)=abs(image.at<unsigned char>(j,i)-grayThresh.at<unsigned char>(j,i));

			 if (image.at<unsigned char>(j,i)>grayThresh.at<unsigned char>(j,i)+ThInTh)
				 	result.at<unsigned char>(j,i)=255;
			 else
				 if ((image.at<unsigned char>(j,i)<grayThresh.at<unsigned char>(j,i)-ThInTh)||(ThInTh<0))
					result.at<unsigned char>(j,i)=0;
				 else
					result.at<unsigned char>(j,i)=grayThresh.at<unsigned char>(j,i);

		 }


    grayThresh.release();
	//	result.convertTo( _result, result.depth() );
	//	criterion.convertTo(_criterion,criterion.depth());
}


void MinMax(InputArray _image,int MinMax[2])
{
	Size size0=_image.size();
	Mat image=_image.getMat();
	int w = size0.width;
	int h = size0.height;
	MinMax[0] = 255;
	MinMax[1] = 0;
	for (int i = 0; i<h;i++)
	{
		for (int j = 0; j<w;j++)
		{
			
			int u = image.at<unsigned char>(i, j);
			if(u < MinMax[0])
				MinMax[0] = u;
			if(u > MinMax[1])
				MinMax[1] = u;
		}
	}
}


void MeanLowHigh(InputArray _image, double Mean[2], double T)
{
	Mat image=_image.getMat();

	Size size0=image.size();
	int w = size0.width;
	int h = size0.height;

	Mean[0] = 0;
	Mean[1] = 0;
	int num1 = 0;
	for (int i = 0; i<h;i++)
	{
		for (int j = 0; j<w;j++)
		{
			int u = image.at<unsigned char>(i, j);
			if(u < T)
			{
				Mean[0] += u;
				num1++;
			}
			else
				Mean[1] += u;
		}
	}
	Mean[0] = Mean[0]*1.0/num1;
	Mean[1] = Mean[1]*1.0/(w*h - num1);
}

void Otsu( InputArray image, OutputArray dst)
{

    int minMax[2] = { 0, 0 };
	double mean[2] = { 0, 0};
	MinMax(image, minMax);
	if(minMax[0] == minMax[1])
    {
        dst.create(image.size(), image.type());
        return;
    }
	double T = 0.5 * (minMax[0] + minMax[1]);
	bool done = false;
	while (!done)
	{
		MeanLowHigh(image, mean, T);
		double Tnext = 0.5 * (mean[0] + mean[1]);
		done = (abs(T-Tnext) < 0.5);
		T = Tnext;
	}


	threshold(image,dst,T,255,CV_THRESH_BINARY);
    return;
}

void windowAdjust(InputArray _image,OutputArray _result,OutputArray _criterion,int radius,int rangeThresh)
{
	Mat image=_image.getMat();
	Mat result=_result.getMat();
	Mat criterion=_criterion.getMat();

	Mat adaptiveMin,adaptiveMax;
	 adaptiveMin.create(image.size(),image.type());
adaptiveMax.create(image.size(),image.type());

dilate(image, adaptiveMax, getStructuringElement(MORPH_ELLIPSE , Size(radius,radius)));
erode(image, adaptiveMin, getStructuringElement(MORPH_ELLIPSE, Size(radius,radius)));


	 Size iSize=image.size();

	 	 for(int i=0;i<iSize.width;i++)
		 for(int j=0;j<iSize.height;j++)
		 {
			  	 criterion.at<unsigned char>(j,i)=adaptiveMax.at<unsigned char>(j,i)-adaptiveMin.at<unsigned char>(j,i);
			
				 unsigned char range=adaptiveMax.at<unsigned char>(j,i)-adaptiveMin.at<unsigned char>(j,i);
				 if (range>rangeThresh)
				result.at<unsigned char>(j,i)=(unsigned char)((image.at<unsigned char>(j,i)-adaptiveMin.at<unsigned char>(j,i)))*(255/float(range));
				 else
				result.at<unsigned char>(j,i)=255;//adjusted.at<unsigned char>(j,i);
		 }


		 adaptiveMin.release();
		 adaptiveMax.release();
}

void dirEdge(InputArray _src, OutputArray _des,char dir,int ddepth,double delta, Point anchor)
{
	//function is incomplete. add kernel creation later.
	Mat src=_src.getMat();
	Mat des=_des.getMat();

	//dir=0 --> horizontal, dir=1 -->vertical
	  /// Update kernel size
  int kernel_size = 7;

		float kernel[] = 
		{-1,-1,-1,-2,-1,-1,-1,
		-1,-1,-2,-3,-2,-1,-1,
		-1,-2,-3,-4,-3,-2,-1,
		0,0,0,0,0,0,0,
		1,2,3,4,3,2,1,
		1,1,2,3,2,1,1,
		1,1,1,2,1,1,1};

	Mat HHor = Mat(kernel_size,kernel_size,CV_32FC1,kernel)/float(4);

	if(dir==1)
		HHor=HHor.t();

		Mat EHor1,EHor2;
	 EHor1.create(src.size(),src.type());
     EHor2.create(src.size(),src.type());


	filter2D(src, EHor1, ddepth, HHor, anchor );
	filter2D(src, EHor2, ddepth, -HHor, anchor );
	des=EHor1+EHor2;

	EHor1.release();
	EHor2.release();
	HHor.release();
}


void blackHatThreshold(Mat& image,Mat&  result, Mat& criterion, int ThInTh, int windowSize,int sigma, int morph_size, int medianSize)
{

	windowThreshold(image,result,criterion,ThInTh, windowSize,sigma);


	medianBlur(result,result, medianSize);

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

	morphologyEx(result,result,MORPH_BLACKHAT,element,Point(-1,-1),5);
	result=(1 << depthToBits( result.depth()))-result;

	windowThreshold(result,result,criterion,-1, windowSize,sigma);
}

void adjustedBinary1(InputArray _image, OutputArray _result,OutputArray _criterion,int radius,int rangeThresh)
{
	Mat image=_image.getMat();
	Mat result=_result.getMat();
	Mat criterion=_criterion.getMat();

	windowAdjust(image,result,criterion,radius,rangeThresh);

	Otsu(result,result);
	
}

void adjustedBinary2(InputArray _image, OutputArray _result,OutputArray _criterion,int* radiusSet,int rangeThresh)
{
	int Num=sizeof(radiusSet)/sizeof(int);
	Mat image=_image.getMat();
	Mat result=_result.getMat();
	Mat criterion=_criterion.getMat();
	Mat minioutputs;
    minioutputs.create(image.size(),image.type());
	result=Mat::zeros(result.size(),result.type());
	for(int i=0;i<Num;i++)
	{

	windowAdjust(image,minioutputs,criterion,radiusSet[i],rangeThresh);

	Otsu(minioutputs,minioutputs);
	result=result|minioutputs;
	}

	minioutputs.release();
}
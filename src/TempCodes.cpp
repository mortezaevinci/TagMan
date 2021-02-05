/*


// define a trackbar callback
static void onTrackbar2(int, void*)
{

        double lowFound, highFound;
        stretchlimits(gray, &lowFound, &highFound, lowFloat/100.0, highFloat/100.0);
        contrastadjust(gray, adjusted, lowFound, highFound);
    
		cout<<lowFound<<" "<<highFound<<endl;

    imshow("contrastadjust", adjusted);
}




// define a trackbar callback
static void onTrackbar(int, void*)
{
    blur(gray, edge, Size(3,3));

    // Run the edge detector on grayscale
    Canny(edge, edge, edgeThresh, edgeThresh*3, 7);
    cedge = Scalar::all(0);

    image.copyTo(cedge, edge);
    imshow("Edge map", cedge);
}

tempMain1fromtest1
{


	    cedge.create(image.size(), image.type());

    // Create a window
    namedWindow("Edge map", 1);

    // create a toolbar
    createTrackbar("Canny threshold", "Edge map", &edgeThresh, 100, onTrackbar);

    // Show the image
    onTrackbar(0, 0);

    // Wait for a key stroke; the same function arranges events processing
    waitKey(0);
	


//example of binary image from a simple threshold
	 Mat imageB;
	 imageB.create(gray.size(), gray.type());

	 threshold(gray,imageB,160,255,CV_THRESH_BINARY);

	 namedWindow("Binary image simple",1);
	 imshow("Binary image simple", imageB);

	 //Otsu (testing for nonadpative image)
	 Mat grayOtsu;
	 grayOtsu.create(gray.size(),gray.type());

	 Otsu(gray,grayOtsu);

	 namedWindow("Otsu",1);
	 imshow("Otsu",grayOtsu);
	 imwrite("result/otsu.jpg",grayOtsu);

	 	 
	 //window-based binary thresholding 1, simplest.
	 int threshWindowSize;
	 threshWindowSize=(int)(25*resizerate);
	 threshWindowSize=threshWindowSize+(threshWindowSize%2)-1;

	 float sigma=5;//0.3*(ksize.height/2 - 1) + 0.8;  //commented one is a recommended case.
	 
	 Mat adaptiveGrayB,adaptiveCriterionB;
	 //Criterion represents where important information are located.

	 adaptiveGrayB.create(gray.size(),gray.type());
	 adaptiveCriterionB.create(gray.size(),gray.type());

	 windowThreshold(gray,adaptiveGrayB,adaptiveCriterionB,-1, threshWindowSize,sigma);

	 namedWindow("adaptiveGrayB 1",1);
	 imshow("adaptiveGrayB 1",adaptiveGrayB);
	 imwrite("result/adaptiveGrayB1.jpg",adaptiveGrayB);

	 
	 //window-based binary thresholding 2

	 int ThInTh=10;
	
	 windowThreshold(gray,adaptiveGrayB,adaptiveCriterionB,ThInTh, threshWindowSize,sigma);

	 namedWindow("adaptiveGrayB 2",1);
	 imshow("adaptiveGrayB 2",adaptiveGrayB);
	 imwrite("result/adaptiveGrayB2.jpg",adaptiveGrayB);


	 //window-based binary thresholding 3 with blackhat (incomplete)

	 int morph_size=5*resizerate;

	blackHatThreshold(gray,adaptiveGrayB,adaptiveCriterionB,ThInTh,threshWindowSize,sigma,morph_size);

	 namedWindow("adaptiveGrayB blackhat",1);
	 imshow("adaptiveGrayB blackhat",adaptiveGrayB);
	  imwrite("result/adaptiveGrayB blackhat.jpg",adaptiveGrayB);

	  	  //nonlinear window-based adjusted binarization 1
		 
	 int MinMaxRadious=(int)(9*resizerate);

	 adjustedBinary1(gray,adaptiveGrayB,adaptiveCriterionB,MinMaxRadious,25);

	 namedWindow("adjustedBinary1",1);
	 imshow("adjustedBinary1",adaptiveGrayB);
	 imwrite("result/adjustedBinary1.jpg",adaptiveGrayB);
}

	 	  //nonlinear window-based adjusted binarization 2
		 
	 int MinMaxRadiousSet2[3]={(int)(5*resizerate),(int)(9*resizerate),(int)(15*resizerate)};

	 adjustedBinary2(gray,adaptiveGrayB,adaptiveCriterionB,MinMaxRadiousSet2,25);

	 namedWindow("adjustedBinary2set2",1);
	 imshow("adjustedBinary2set2",adaptiveGrayB);
	 imwrite("result/adjustedBinary2set2.jpg",adaptiveGrayB);

*/
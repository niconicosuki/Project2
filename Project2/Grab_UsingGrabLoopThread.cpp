// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

//include files to use OpenCV API
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include files used by samples.
#include "ConfigurationEventPrinter.h"
#include "ImageEventPrinter.h"

#include "TalkToCamera.h"



// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Namespace for using opencv objects.
using namespace cv;

int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized. 
    PylonInitialize();

    try
    {
        // Create an instant camera object for the camera device found first.
        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

		CGrabResultPtr ptrGrabResult;

		GenApi::INodeMap& nodemap = camera.GetNodeMap();

		GenApi::CIntegerPtr width = nodemap.GetNode("Width");
		GenApi::CIntegerPtr height = nodemap.GetNode("Height");


		CImageFormatConverter formatConverter;

		formatConverter.OutputPixelFormat = PixelType_BGR8packed;

		CPylonImage pylonImage;

		Mat OpenCvImage;

		VideoWriter cvVideoCreator;

		string videoFileName = "testfile.avi";

		Size framesize = Size((int)width->GetValue(),(int)height->GetValue());

		int codec = CV_FOURCC('M', 'J', 'P', 'G');
		double fps = 24;

		cvVideoCreator.open(videoFileName, codec, fps, framesize, 1);

		if (!cvVideoCreator.isOpened()) {
			cerr << "could not open out put video file for writing\n" << endl;
		}

		namedWindow("test");

		init_camera(camera);

		grab_frame(camera,ptrGrabResult);

		if (ptrGrabResult->GrabSucceeded())
		{
			cout << "sizeX: " << ptrGrabResult->GetWidth() << endl;
			cout << "sizeY: " << ptrGrabResult->GetHeight() << endl;
			formatConverter.Convert(pylonImage, ptrGrabResult);
			OpenCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());
			
			cvVideoCreator.write(OpenCvImage);

			imshow("test", OpenCvImage);

		

		}
       
		else {
			cout << "something wrong happened, can't get the grab result or link error" << endl;
		}
         
    }
    catch (const GenericException &e)
    {	
		error_handle( e);
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press Enter to exit." << endl;
    while( cin.get() != '\n');

    // Releases all pylon resources. 
    PylonTerminate(); 

    return exitCode;
}

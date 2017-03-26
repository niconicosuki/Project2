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


#define USE_USB 

#if defined( USE_1394 )
// Settings for using Basler IEEE 1394 cameras.
#include <pylon/1394/Basler1394InstantCamera.h>
typedef Pylon::CBasler1394InstantCamera Camera_t;
typedef Pylon::CBasler1394ImageEventHandler ImageEventHandler_t; // Or use Camera_t::ImageEventHandler_t
typedef Pylon::CBasler1394GrabResultPtr GrabResultPtr_t; // Or use Camera_t::GrabResultPtr_t
using namespace Basler_IIDC1394CameraParams;
#elif defined ( USE_GIGE )
// Settings for using Basler GigE cameras.
#include <pylon/gige/BaslerGigEInstantCamera.h>
typedef Pylon::CBaslerGigEInstantCamera Camera_t;
typedef Pylon::CBaslerGigEImageEventHandler ImageEventHandler_t; // Or use Camera_t::ImageEventHandler_t
typedef Pylon::CBaslerGigEGrabResultPtr GrabResultPtr_t; // Or use Camera_t::GrabResultPtr_t
using namespace Basler_GigECameraParams;
#elif defined( USE_USB )
// Settings for using Basler USB cameras.
#include <pylon/usb/BaslerUsbInstantCamera.h>
typedef Pylon::CBaslerUsbInstantCamera Camera_t;
typedef Pylon::CBaslerUsbImageEventHandler ImageEventHandler_t; // Or use Camera_t::ImageEventHandler_t
typedef Pylon::CBaslerUsbGrabResultPtr GrabResultPtr_t; // Or use Camera_t::GrabResultPtr_t
using namespace Basler_UsbCameraParams;
#else

#endif



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
        Camera_t camera( CTlFactory::GetInstance().CreateFirstDevice());

		//ptrgrabresult is a smart ptr that about the grab result
		CGrabResultPtr ptrGrabResult;

		//format converter and pylonimage is about transform the pylon picture into opencv format
		CImageFormatConverter formatConverter;

		formatConverter.OutputPixelFormat = PixelType_BGR8packed;

		CPylonImage pylonImage;

		Mat OpenCvImage;


		namedWindow("test");

		init_camera(camera);//open the camera

		/*
		this part mainly about create a video writer, to save images as a video
		
		*/
		VideoWriter cvVideoCreator;

		string videoFileName = "testfile.avi";
		GenApi::INodeMap& nodemap = camera.GetNodeMap();

		GenApi::CIntegerPtr width = nodemap.GetNode("Width");
		GenApi::CIntegerPtr height = nodemap.GetNode("Height");

		Size framesize = Size((int)width->GetValue(), (int)height->GetValue());

		int codec = CV_FOURCC('M', 'J', 'P', 'G');
		double fps = 24;

		cvVideoCreator.open(videoFileName, codec, fps, framesize, 1);

		if (!cvVideoCreator.isOpened()) {
			cerr << "could not open out put video file for writing\n" << endl;
		}


		//this part is about the chunk property of cameras
		if (GenApi::IsWritable(camera.ChunkModeActive))
		{
			camera.ChunkModeActive.SetValue(true);
		}
		else
		{
			throw RUNTIME_EXCEPTION("The camera doesn't support chunk features");
		}



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

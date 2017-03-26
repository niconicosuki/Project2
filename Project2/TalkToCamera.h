#pragma once
// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Include files used by samples.
#include "ConfigurationEventPrinter.h"
#include "ImageEventPrinter.h"

//Example of an image event handler.
class CSampleImageEventHandler : public Pylon::CImageEventHandler
{
public:
	virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult)
	{
#ifdef PYLON_WIN_BUILD
		// Display the image
		Pylon::DisplayImage(1, ptrGrabResult);
#endif

		std::cout << "CSampleImageEventHandler::OnImageGrabbed called." << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
};


//init_camera & grab frame are "grab loop thread" strategy
void init_camera(Pylon::CInstantCamera& camera);

void grab_frame(Pylon::CInstantCamera& camera, Pylon::CGrabResultPtr& ptrGrabResult);


void error_handle(const GenericException &e);
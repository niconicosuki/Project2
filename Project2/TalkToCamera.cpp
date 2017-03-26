#pragma once
#include "TalkToCamera.h"

using namespace Pylon;
using namespace std;

void init_camera(CInstantCamera& camera) {

	// Register the standard configuration event handler for enabling software triggering.
	// The software trigger configuration handler replaces the default configuration
	// as all currently registered configuration handlers are removed by setting the registration mode to RegistrationMode_ReplaceAll.
	camera.RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);

	// For demonstration purposes only, add a sample configuration event handler to print out information
	// about camera use.
	camera.RegisterConfiguration(new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete);

	// The image event printer serves as sample image processing.
	// When using the grab loop thread provided by the Instant Camera object, an image event handler processing the grab
	// results must be created and registered.
	camera.RegisterImageEventHandler(new CImageEventPrinter, RegistrationMode_Append, Cleanup_Delete);

	// For demonstration purposes only, register another image event handler.
	camera.RegisterImageEventHandler(new CSampleImageEventHandler, RegistrationMode_Append, Cleanup_Delete);

	// Open the camera device.
	camera.Open();

};

void grab_frame(CInstantCamera& camera, CGrabResultPtr& ptrGrabResult) {

	// Can the camera device be queried whether it is ready to accept the next frame trigger?
	if (camera.CanWaitForFrameTriggerReady())
	{
		// Start the grabbing using the grab loop thread, by setting the grabLoopType parameter
		// to GrabLoop_ProvidedByInstantCamera. The grab results are delivered to the image event handlers.
		// The GrabStrategy_OneByOne default grab strategy is used.
		camera.StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);

		camera.RetrieveResult(500, ptrGrabResult, TimeoutHandling_ThrowException);

		cerr << endl << "Enter \"t\" to trigger the camera or \"e\" to exit and press enter? (t/e)" << endl << endl;
		 
		// Wait for user input to trigger the camera or exit the program.
		// The grabbing is stopped, the device is closed and destroyed automatically when the camera object goes out of scope.
		char key;
		do
		{
			cin.get(key);
			if ((key == 't' || key == 'T'))
			{
				// Execute the software trigger. Wait up to 500 ms for the camera to be ready for trigger.
				if (camera.WaitForFrameTriggerReady(500, TimeoutHandling_ThrowException))
				{
					camera.ExecuteSoftwareTrigger();
				}
			}
		} while ((key != 'e') && (key != 'E'));
	}
	else
	{
		// See the documentation of CInstantCamera::CanWaitForFrameTriggerReady() for more information.
		cout << endl;
		cout << "This sample can only be used with cameras that can be queried whether they are ready to accept the next frame trigger.";
		cout << endl;
		cout << endl;
	}

};

void error_handle(const GenericException &e) {
	// Error handling.
	cerr << "An exception occurred." << endl
		<< e.GetDescription() << endl;
	//exitCode = 1;

	// Remove left over characters from input buffer.
	cin.ignore(cin.rdbuf()->in_avail());

};
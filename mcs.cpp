/**********************************************************************
* 
* ***** Multi Spectral Camera Image Capture Program *****
* 
* Author: Prashant Kumar
* 
* command to compile this program using terminal:
* g++ mcs.cpp -lboost_filesystem -lboost_system -lboost_thread -lueye_api -lpthread -I mavlink mavlink/serial_port.cpp mavlink/autopilot_interface.cpp -o mcs
* 
* Manual for ueye functions
* https://en.ids-imaging.com/manuals/uEye_SDK/EN/uEye_Manual_4.90.6/index.html
* 
* 
***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ueye.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include "mavlink/mavlink_control.h"

using namespace std;
#include "boost/date_time/posix_time/posix_time.hpp" 
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;

//https://gist.github.com/ChristianUlbrich/7871863#file-test_frame_capture-c

bool log_stuff = false;
static int imgNo = 0;
static string save_dir_base = "/mnt/dr1/images/";
static string save_directory = "";
static bool ok = true;
static Time t_cycle;
ofstream logFile, notesFile, gpsLogfile;
string notesFileName;
const int nCams = 6;
HIDS hCamIds [nCams];// = { 1, 2, 3, 4, 5, 6 };
bool useCamIds [nCams];// = { true, true, true, true, true, true };

//mavlink vars
bool use_mavlink = true;
mavlink_local_position_ned_t lpos;
mavlink_global_position_int_t gpos;
mavlink_attitude_t att;
Autopilot_Interface *api;
string MAVLinkPort = "/dev/ttyUSB0";
int baudrate = 115200;

string InitCameraRetStr(INT nRet){
	string retStrA;
	switch(nRet)
	{
		case IS_ALL_DEVICES_BUSY:
		retStrA = "All cameras are in use"; break;
		case IS_BAD_STRUCTURE_SIZE:
		retStrA = "An internal structure has an incorrect size."; break;
		case IS_CANT_ADD_TO_SEQUENCE:
		retStrA = "The image memory is already included in the sequence and cannot be added again."; break;
		case IS_CANT_COMMUNICATE_WITH_DRIVER:
		retStrA = "Communication with the driver failed because no driver has been loaded."; break;
		case IS_CANT_OPEN_DEVICE:
		retStrA = "An attempt to initialize or select the camera failed (no camera connected or initialization error)."; break;
		case IS_CANT_OPEN_REGISTRY:
		retStrA = "Error opening a Windows registry key"; break;
		case IS_CANT_READ_REGISTRY:
		retStrA = "Error reading settings from the Windows registry"; break;
		case IS_CAPTURE_RUNNING:
		retStrA = "A capturing operation is in progress and must be terminated first."; break;
		case IS_CRC_ERROR:
		retStrA = "A CRC error-correction problem occurred while reading the settings."; break;
		case IS_DEVICE_ALREADY_PAIRED:
		retStrA = "The device is already paired."; break;
		case IS_DEVICE_NOT_COMPATIBLE:
		retStrA = "The device is not compatible to the drivers."; break;
		case IS_DR_CANNOT_CREATE_SURFACE:
		retStrA = "The image surface or overlay surface could not be created."; break;
		case IS_DR_CANNOT_CREATE_TEXTURE:
		retStrA = "The texture could not be created."; break;
		case IS_DR_CANNOT_CREATE_VERTEX_BUFFER:
		retStrA = "The vertex buffer could not be created."; break;
		case IS_DR_DEVICE_OUT_OF_MEMORY:
		retStrA = "Not enough graphics memory available."; break;
		case IS_DR_LIBRARY_NOT_FOUND:
		retStrA = "The DirectRenderer library could not be found."; break;
		case IS_ERROR_CPU_IDLE_STATES_CONFIGURATION:
		retStrA = "The configuration of the CPU idle has failed."; break;
		case IS_FILE_WRITE_OPEN_ERROR:
		retStrA = "File cannot be opened for writing or reading."; break;
		case IS_INCOMPATIBLE_SETTING:
		retStrA = "Because of other incompatible settings the function is not possible."; break;
		case IS_INVALID_BUFFER_SIZE:
		retStrA = "The image memory has an inappropriate size to store the image in the desired format."; break;
		case IS_INVALID_CAMERA_TYPE:
		retStrA = "The camera type defined in the .ini file does not match the current camera model."; break;
		case IS_INVALID_CAPTURE_MODE:
		retStrA = "The function can not be executed in the current camera operating mode (free run, trigger or standby)."; break;
		case IS_INVALID_DEVICE_ID:
		retStrA = "The device ID is invalid. Valid IDs start from 1 for USB cameras, and from 1001 for GigE cameras."; break;
		case IS_INVALID_EXPOSURE_TIME:
		retStrA = "This setting is not available for the currently set exposure time."; break;
		case IS_INVALID_CAMERA_HANDLE:
		retStrA = "Invalid camera handle"; break;
		case IS_INVALID_IP_CONFIGURATION:
		retStrA = "The configuration of the IP address is invalid."; break;
		case IS_INVALID_MEMORY_POINTER:
		retStrA = "Invalid pointer or invalid memory ID"; break;
		case IS_INVALID_MODE:
		retStrA = "Camera is in standby mode, function not allowed"; break;
		case IS_INVALID_PARAMETER:
		retStrA = "One of the submitted parameters is outside the valid range or is not supported for this sensor or is not available in this mode."; break;
		case IS_INVALID_PIXEL_CLOCK:
		retStrA = "This setting is not available for the currently set pixel clock frequency."; break;
		case IS_IO_REQUEST_FAILED:
		retStrA = "An IO request from the uEye driver failed. Possibly the versions of the ueye_api.dll (API) and the driver file (ueye_usb.sys or ueye_eth.sys) do not match."; break;
		case IS_NETWORK_CONFIGURATION_INVALID:
		retStrA = "The configuration of the network card is invalid."; break;
		case IS_NETWORK_FRAME_SIZE_INCOMPATIBLE:
		retStrA = "The settings for the image size of the camera are not compatible to the PC network card."; break;
		case IS_NO_ACTIVE_IMG_MEM:
		retStrA = "No active image memory available. You must set the memory to active using the is_SetImageMem() function or create a sequence using the is_AddToSequence() function."; break;
		case IS_NO_IMAGE_MEM_ALLOCATED:
		retStrA = "The driver could not allocate memory."; break;
		case IS_NO_IR_FILTER:
		retStrA = "No IR filter available"; break;
		case IS_NO_SUCCESS:
		retStrA = "General error message"; break;
		case IS_NOT_CALIBRATED:
		retStrA = "The camera does not contain any calibration data."; break;
		case IS_NOT_SUPPORTED:
		retStrA = "The camera model used here does not support this function or setting."; break;
		case IS_NULL_POINTER:
		retStrA = "Invalid array"; break;
		case IS_OUT_OF_MEMORY:
		retStrA = "No memory could be allocated."; break;
		case IS_SEQUENCE_BUF_ALREADY_LOCKED:
		retStrA = "The memory could not be locked. The pointer to the buffer is invalid."; break;
		case IS_STARTER_FW_UPLOAD_NEEDED:
		retStrA = "The camera's starter firmware is not compatible with the driver and needs to be updated."; break;
		case IS_SUCCESS:
		retStrA = "Function executed successfully"; break;
		case IS_SUBNET_MISMATCH:
		retStrA = "The subnet of the camera and PC network card are different."; break;
		case IS_SUBNETMASK_MISMATCH:
		retStrA = "The subnet mask of the camera and PC network card are different."; break;
		case IS_TIMED_OUT:
		retStrA = "A timeout occurred. An image capturing process could not be terminated within the allowable period."; break;
		case IS_TRIGGER_ACTIVATED:
		retStrA = "The function cannot be used because the camera is waiting for a trigger signal."; break;
		default:
		retStrA = "is_InitCamera retrun str not found";break;
	}
	return retStrA;
}

string GetImgMemRetStr(INT nRet){
	string retStrA;
	switch(nRet)
	{
		case IS_CANT_COMMUNICATE_WITH_DRIVER:
		retStrA = "Communication with the driver failed because no driver has been loaded."; break;
		case IS_CANT_OPEN_DEVICE:
		retStrA = "An attempt to initialize or select the camera failed (no camera connected or initialization error)."; break;
		case IS_INVALID_CAMERA_HANDLE:
		retStrA = "Invalid camera handle."; break;
		case IS_INVALID_MEMORY_POINTER:
		retStrA = "Invalid pointer or invalid memory ID."; break;
		case IS_INVALID_PARAMETER:
		retStrA = "One of the submitted parameters is outside the valid range or is not supported for this sensor or is not available in this mode."; break;
		case IS_IO_REQUEST_FAILED:
		retStrA = "An IO request from the uEye driver failed. Possibly the versions of the ueye_api.dll (API) and the driver file (ueye_usb.sys or ueye_eth.sys) do not match."; break;
		case IS_NO_SUCCESS:
		retStrA = "General error message."; break;
		case IS_SUCCESS:
		retStrA = "Function executed successfully."; break;
		default:
		retStrA = "is_GetImageMem retrun str not found";break;
	}
	return retStrA;
}

string WaitEventRetStr(INT nRet){
	string retStrA;
	switch(nRet)
	{
		case IS_NO_SUCCESS:
		retStrA = "General error message"; break;
		case IS_SUCCESS:
		retStrA = "Function executed successfully"; break;
		case IS_TIMED_OUT:
		retStrA = "A timeout occurred."; break;
		default:
		retStrA = "is_WaitEvent retrun str not found";break;
	}
	return retStrA;
}

int setupCamFn(HIDS hCam) {
	uint initial_hCam = hCam;
	cout << " _settingUpCamID_" << hCam << "_";
	logFile << " _settingUpCamID_" << hCam << "_";
	
	bool success = false;
	//Kamera öffnen
	INT nRet = is_InitCamera(&hCam, NULL);
	cout << hCam << "_";
	logFile << hCam << "_";
	
	string retStr = InitCameraRetStr(nRet);
	logFile << "Status Init:" << nRet << ":" << retStr << endl;
	if(log_stuff) cout << "Status Init:" << nRet << ":" << retStr << endl;

	//Pixel-Clock setzen
	UINT nPixelClock;
	nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET, (void*)&nPixelClock, sizeof(nPixelClock));
	logFile << "Status is_PixelClock GET " << nRet << ", nPixelClock " << nPixelClock << endl;
	if(log_stuff) printf("Status is_PixelClock GET %d, nPixelClock %d\n",nRet,nPixelClock);
	nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClock, sizeof(nPixelClock));
	logFile << "Status is_PixelClock SET " << nRet << endl;
	if(log_stuff) printf("Status is_PixelClock SET %d\n",nRet);
	
	//Farbmodus der Kamera setzen
	//INT colorMode = IS_CM_CBYCRY_PACKED;
	INT colorMode = IS_CM_BGR8_PACKED; //IS_CM_BGR8_PACKED;

	nRet = is_SetColorMode(hCam,colorMode);
	logFile << "Status SetColorMode " << nRet << endl;
	if(log_stuff) printf("Status SetColorMode %d\n",nRet);
	
	UINT formatID = 1;
	//Bildgröße einstellen -> 2592x1944
	nRet = is_ImageFormat(hCam, IMGFRMT_CMD_SET_FORMAT, &formatID, 1);
	logFile << "Status ImageFormat (125 is ok) " << nRet << endl;
	if(log_stuff) printf("Status ImageFormat (125 is ok) %d\n",nRet);
	if(nRet == 125)
		success = true;
	
	//Speicher für Bild alloziieren
	int memID = 0;
	char* pMem = NULL;
	nRet = is_AllocImageMem(hCam, 1280, 1024, 24, &pMem, &memID);
	logFile << "Status AllocImage " << nRet << endl;
	if(log_stuff) printf("Status AllocImage %d\n",nRet);

	//diesen Speicher aktiv setzen
	nRet = is_SetImageMem(hCam, pMem, memID);
	logFile << "Status SetImageMem " << nRet << endl;
	if(log_stuff) printf("Status SetImageMem %d\n",nRet);
	
	//Bilder im Kameraspeicher belassen
	INT displayMode = IS_SET_DM_DIB;
	nRet = is_SetDisplayMode (hCam, displayMode);
	logFile << "Status displayMode " << nRet << endl;
	if(log_stuff) printf("Status displayMode %d\n",nRet);

	//putting in auto exposure
	double dEnable = 1;
	nRet = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0 );
	logFile << "Status AutoExposure " << nRet << endl;
	if(log_stuff) printf("Status AutoExposure %d\n",nRet);
	//IS_SET_ENABLE_AUTO_SENSOR_SHUTTER
	
	//putting in auto gain
	double dEnable2 = 1;
	nRet = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &dEnable2, 0 );
	logFile << "Status AutoGain " << nRet << endl;
	if(log_stuff) printf("Status AutoGain %d\n",nRet);

	//setting to external Hardware Trigger mode with Rising Signal Edge
	is_SetExternalTrigger(hCam, IS_SET_TRIGGER_HI_LO);
	logFile << "Status is_SetExternalTrigger " << nRet << endl;
	if(log_stuff) printf("Status is_SetExternalTrigger %d\n",nRet);
	
	is_EnableEvent(hCam, IS_SET_EVENT_FRAME);

	//continous capture and transfer of images to image memory
	nRet = is_CaptureVideo(hCam, IS_DONT_WAIT);
	logFile << "Status is_CaptureVideo " << nRet << endl;
	if(log_stuff) printf("Status is_CaptureVideo %d\n",nRet);
	
	logFile << "cam" << hCam << " params set. " << (success ? "success" : "failure") << endl << endl;
	cout << "cam" << hCam << " params set. " << (success ? "success" : "failure") << endl;
	//usleep(10*1000);
	
	if(initial_hCam != hCam)
	{
		cout << "_Fault_hCamId_changed_from_" << initial_hCam << "_to_" << hCam << endl;
		logFile << "_Fault_hCamId_changed_from_" << initial_hCam << "_to_" << hCam << endl << endl;
		success = false;
	}
	
	return success;
}

bool captureImg(HIDS hCam) {
	Time t1(boost::posix_time::microsec_clock::local_time());
	
	INT nRet = is_WaitEvent(hCam, IS_SET_EVENT_FRAME, 5000);
	
	Time t2(boost::posix_time::microsec_clock::local_time());
	TimeDuration dt1 = t2 - t1;
	long msec1 = dt1.total_milliseconds(); 
	if(msec1 > 5) 	//in general fetch time is less than 1ms, so anything more than 5ms is for sure next image count
	{
		dt1 = t2 - t_cycle;		//cycle time
		msec1 = dt1.total_milliseconds();  
		++imgNo;
		cout << "\n" << msec1 << "ms\tImgNo_" << imgNo << "___";
		logFile << "\n" << msec1 << "ms\tImgNo_" << imgNo << "___";
		t_cycle = t2;
	}
	
	if (nRet == IS_SUCCESS)
	{
		string str = save_directory + "cam" + boost::lexical_cast<string>(hCam) + "/" + boost::lexical_cast<string>(imgNo) + ".bmp";
		wchar_t* widecstr = new wchar_t[str.length()];
		for(int i=0; i<str.length(); i++)
			widecstr[i] = str[i];
		
		/* event signalled */
		IMAGE_FILE_PARAMS ImageFileParams;
		ImageFileParams.pwchFileName = widecstr;
		ImageFileParams.pnImageID = NULL;
		ImageFileParams.ppcImageMem = NULL;
		ImageFileParams.nQuality = 0;
		ImageFileParams.nFileType = IS_IMG_BMP; //JPG BMP PNG
		INT nRet = is_ImageFile(hCam, IS_IMAGE_FILE_CMD_SAVE, (void*) &ImageFileParams, sizeof(ImageFileParams));
		if(nRet == 0)
		{
			cout << "_Cam" << hCam << "_";
			logFile << "_Cam" << hCam << "_";
		}
		else
		{
			cout << "\nCam" << hCam << "_Status(0 is ok):" << nRet << endl;
			logFile << "\nCam" << hCam << "_Status(0 is ok):" << nRet << endl;
		}
	}
	else
	{
		cout << "\nCam" << hCam << "__is_WaitEvent_Failure" << nRet << endl;
		logFile << "\nCam" << hCam << "__is_WaitEvent_Failure" << nRet << endl;
		string retStr = WaitEventRetStr(nRet);
		cout << "is_WaitEvent: " << retStr << endl;
		logFile << "is_WaitEvent: " << retStr << endl;
		return false;
	}
	return true;
}

void exitCameras(HIDS hCam) {
	cout << "Exit camera: " << hCam << "...";
	logFile << "Exit camera: " << hCam << "...";
	is_DisableEvent(hCam, IS_SET_EVENT_FRAME);
	is_ExitCamera(hCam);
	cout << " Exited." << endl;
	logFile << " Exited." << endl;
}

void closeProgramWaitingFunction() {
	char keypressed;
	while(ok)
	{
		cin >> keypressed;
		if( keypressed == 27 )
		{
			cout << "\nEcs key pressed.." << endl;
			logFile << "\nEcs key pressed.." << endl;
			ok = false;
			break;
		}
		else
			cout << "Key Pressed: " << keypressed << endl;
	}
}

int initializationSteps(int argc, char* argv[]) {
	cout << "\n***** Multi Spectral Camera Image Capture Program *****\n" << endl;
	cout << "Important notes:" << endl;
	cout << "1. If cameras do not initialize then either remove camera USBs for 5 seconds or shutdown computer, disconnect power, reconnect power and restart computer." << endl;
	cout << "2. Folder '/mnt/dr1/images' should exist. Additional folder with current date time will be created and all images will be saved in it." << endl;
	cout << "3. To print everything use the command line: './mcs --log' (not recommended)." << endl;
	cout << "4. To use single camera, use './mcs --cam n', n being camera number 1-6." << endl;
	cout << "5. To stop program, press 'Esc' key and press Enter." << endl;
	cout << "6. Provide notes at start and end of program when asked for your future reference. Notes will be saved in file notes.txt" << endl;
	cout << "7. To not use mavlink use './mcs --no_mavlink'." << endl;
	cout << endl;
	usleep(1000*1000);
	
	//directory name is date time
	std::time_t rawtime;
	std::tm* timeinfoA;
	char buffer [80];
	std::time(&rawtime);
	timeinfoA = std::localtime(&rawtime);
	std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfoA);
	save_directory = save_dir_base + buffer + "/";

	//base time = create tm with today's date
	std::tm timeinfo = std::tm();
	timeinfo.tm_year = timeinfoA->tm_year;
	timeinfo.tm_mon = timeinfoA->tm_mon;
	timeinfo.tm_mday = timeinfoA->tm_mday;
	std::time_t tt = std::mktime (&timeinfo);
	
	boost::filesystem::path dir(save_directory);
	if(boost::filesystem::create_directory(dir)) {
		std::cout << "Created save directory: " << save_directory << "\n";
	}
	else {
		std::cout << "Could not create save directory! " << save_directory << "\n";
		return -1;
	}
	
	//log file
	string logFileName = save_directory + "log.txt";
	logFile.open(logFileName.c_str(), ios_base::app);
	logFile << save_directory << endl;
	logFile << "Arguments: ";
	for (uint i = 0; i < argc; i++)
		logFile << argv[i] << " ";
	logFile << endl;
	
	for (uint i = 0; i < nCams; i++) {
		hCamIds[i] = i+1;
		useCamIds[i] = true;
	}
	
	for (uint i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "--log") {
			log_stuff = true;
		}
		else if(string(argv[i]) == "--no_mavlink") {
			use_mavlink = false;
		}
		else if(string(argv[i]) == "--cam") {
			for (uint j = 0; j < nCams; j++) {
				if(j == atoi(argv[i+1]) -1) {
					cout << "Only use Cam" << j+1 << endl;
				} else {
					useCamIds[j] = false;
				}
			}
		}
	}
	
	cout << "Camera IDs: Usage:\n";
	for (uint i = 0; i < nCams; i++)
	{
		cout << i+1 << " " << (useCamIds[i] ? "true" : "false") << " ";
		logFile << i+1 << " " << (useCamIds[i] ? "true" : "false") << " ";
	}
	cout << endl << endl;
	logFile << endl << endl;
	
	bool allsuccess = true;
	cout << "Setup cameras:" << endl;
	logFile << "Setup cameras:" << endl;
	for (uint i = 0; i < nCams; i++) {
		if(useCamIds[i])
			allsuccess *= setupCamFn(i+1);
	}
	printf("\nAll Camera Setup Result: %s\n", allsuccess ? "success" : "failure");
	
	if(!allsuccess)
	{
		printf("Closing...\n");
		for (uint i = 0; i < nCams; i++)
		{
			if(useCamIds[i])
				exitCameras(i+1);
		}
		return -1;
	}
	
	boost::filesystem::path dir1(save_directory + "cam1/");
	boost::filesystem::path dir2(save_directory + "cam2/");
	boost::filesystem::path dir3(save_directory + "cam3/");
	boost::filesystem::path dir4(save_directory + "cam4/");
	boost::filesystem::path dir5(save_directory + "cam5/");
	boost::filesystem::path dir6(save_directory + "cam6/");
	if(!boost::filesystem::create_directory(dir1) || !boost::filesystem::create_directory(dir2) || !boost::filesystem::create_directory(dir3)
		 || !boost::filesystem::create_directory(dir4) || !boost::filesystem::create_directory(dir5) || !boost::filesystem::create_directory(dir6))
	{
		cout << "Could not create cam* save directories!" << "\n";
		logFile << "Could not create cam* save directories!" << "\n";
		return -1;
	}
	
	//threaded function to stop program
	boost::thread thread_close_prog(closeProgramWaitingFunction);
	
	//comments file
	cout << "\nEnter start comments for this imaging session:" << endl;
	string notes;
	getline(cin,notes);
	notesFileName = save_directory + "notes.txt";
	notesFile.open(notesFileName.c_str(), ios_base::app);
	notesFile << save_directory << endl;
	notesFile << "\nStart comments" << endl;
	notesFile << notes << endl;
	notesFile.close();
	
	return 0;
}

void endProgramStuff() {
	//Kamera wieder freigeben
	cout << endl;
	for (uint i = 0; i < nCams; i++)
	{
		if(useCamIds[i])
			exitCameras(i+1);
	}
	while ((getchar()) != '\n');
		
	cout << "\nEnter end comments for this imaging session:" << endl;
	string notes;
	getline(cin,notes);
	notesFile.open(notesFileName.c_str(), ios_base::app);
	notesFile << "\nEnd comments" << endl;
	notesFile << notes << endl;
	notesFile.close();
	
	logFile.close();
	
	cout << "\nEverything saved at location:\n" << save_directory << endl << endl;
}

// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler( int sig ) {
	printf("Terminating at User Request\n");
	
	// autopilot interface
	try {
		autopilot_interface_quit->handle_quit(sig);
	}
	catch (int error){}

	// serial port
	try {
		serial_port_quit->handle_quit(sig);
	}
	catch (int error){}
	
	gpsLogfile.close();
	endProgramStuff();

	// end program here
	exit(0);
}

void runImageCapture() {
	cout << "\nStart capturing...\n\n\tImgNo_0___";
	logFile << "\nStart capturing...\n\n\tImgNo_0___";
	
	t_cycle = boost::posix_time::microsec_clock::local_time();
	Time t_start = boost::posix_time::microsec_clock::local_time();
	
	bool gpsGlobalCycleBool = false;
	bool imuAttitudeCycleBool = true;
	
	while(ok)
	{
		for (uint i = 0; i < nCams; i++)
		{
			if(useCamIds[i])
			{
				if(!captureImg(i+1))
				{
					cout << "Image not being captured! Exiting..." << endl; 
					logFile << "Image not being captured! Exiting..." << endl; 
					ok = false;
					break;
				}
			}
		}
		
		if(use_mavlink)
		{
			gpos = api->current_messages.global_position_int;
			lpos = api->current_messages.local_position_ned;
			att = api->current_messages.attitude;
			
			gpsLogfile <<imgNo<<","<<","<<att.roll<<","<<att.pitch<<","<<att.yaw<<","<<att.rollspeed<<","<<att.pitchspeed<<","<<att.yawspeed<<","<<gpos.lat<<","<<gpos.lon<<","<<gpos.alt<<","<<gpos.relative_alt<<","<<gpos.vx<<","<<gpos.vy<<","<<gpos.vz<<","<<gpos.hdg<<","<<lpos.x<<","<<lpos.y<<","<<lpos.z<<","<<"\n";
		}

		if(imgNo == 4) {
			Time t_end(boost::posix_time::microsec_clock::local_time());
			TimeDuration dt_loop = t_end - t_start;
			long msec_loop = dt_loop.total_milliseconds();
			if(msec_loop < 500)
			{
				cout << "Capture Loop is running faster than anticipated. Some error is there in camera initialization. Exiting..." << endl; 
				logFile << "Capture Loop is running faster than anticipated. Some error is there in camera initialization. Exiting..." << endl; 
				ok = false;
				break;
			}
		}
		
	}
}

int main(int argc, char* argv[])
{
	int ret = initializationSteps(argc, argv);
	if(ret == -1)
		return ret;
	
	char *uart_name = (char *)MAVLinkPort.c_str();
	Serial_Port serial_port(uart_name, baudrate);
	Autopilot_Interface autopilot_interface(&serial_port);
	if(use_mavlink)
	{
		cout << "baudrate: " << baudrate << endl;
		cout << "MAVLinkPort: " << MAVLinkPort << endl;
		logFile << "baudrate: " << baudrate << endl;
		logFile << "MAVLinkPort: " << MAVLinkPort << endl;
		string gpsLogfileName = save_directory + "gpsLog.txt";
		gpsLogfile.open(gpsLogfileName.c_str(), ios_base::app);
		gpsLogfile << save_directory << endl;
		gpsLogfile <<"imgNo,att.roll,att.pitch,att.yaw,att.rollspeed,att.pitchspeed,att.yawspeed,gpos.lat,gpos.lon,gpos.alt,gpos.relative_alt,gpos.vx,gpos.vy,gpos.vz,gpos.hdg,lpos.x,lpos.y,lpos.z\n";
		
		//mavlink start
		serial_port_quit         = &serial_port;
		autopilot_interface_quit = &autopilot_interface;
		cout << "Start serial port" << endl;
		//signal(SIGINT,quit_handler);
		cout << "Initialize MAVLINK" << endl;
		serial_port.start();
		cout << "Start api" << endl;
		autopilot_interface.start();
		api = &autopilot_interface;
		cout << "MAVLINK initialized" << endl;
		
		runImageCapture();
		
		//mavlink stop
		autopilot_interface.stop();
		serial_port.stop();
	}
	else
		runImageCapture();
	
	endProgramStuff();
	
	return 0;
}

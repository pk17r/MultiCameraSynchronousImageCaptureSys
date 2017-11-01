//http://www.boost.org/doc/libs/1_65_1/more/getting_started/unix-variants.html
//command:
//c++ -I /Desktop/boost_1_65_1 mcs6.cpp -lueye_api -o mcs

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
#include<boost/locale.hpp>
#include<boost/lexical_cast.hpp>
using namespace std;
#include "boost/date_time/posix_time/posix_time.hpp" 
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;

//https://gist.github.com/ChristianUlbrich/7871863#file-test_frame_capture-c

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

void setupCamFn(HIDS hCam) {
  sleep(1);
  //Kamera öffnen
  INT nRet = is_InitCamera (&hCam, NULL);
  printf("Status Init %d\n",nRet);
  string retStr = InitCameraRetStr(nRet);
  cout << retStr + "\n";

  //Pixel-Clock setzen
  UINT nPixelClock;
  nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_GET, (void*)&nPixelClock, sizeof(nPixelClock));
  printf("Status is_PixelClock GET %d, nPixelClock %d\n",nRet,nPixelClock);
  nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClock, sizeof(nPixelClock));
  printf("Status is_PixelClock SET %d\n",nRet);

  //Farbmodus der Kamera setzen
  //INT colorMode = IS_CM_CBYCRY_PACKED;
  INT colorMode = IS_CM_BGR8_PACKED; //IS_CM_BGR8_PACKED;
  
  nRet = is_SetColorMode(hCam,colorMode);
  printf("Status SetColorMode %d\n",nRet);

  UINT formatID = 1;
  //Bildgröße einstellen -> 2592x1944
  nRet = is_ImageFormat(hCam, IMGFRMT_CMD_SET_FORMAT, &formatID, 1);
  printf("Status ImageFormat (125 is ok) %d\n",nRet);
  //status description
  //imageFormatStrFn(nRet);

  //Speicher für Bild alloziieren
  int memID = 0;
  char* pMem = NULL;
  nRet = is_AllocImageMem(hCam, 1280, 1024, 24, &pMem, &memID);
  printf("Status AllocImage %d\n",nRet);

  //diesen Speicher aktiv setzen
  nRet = is_SetImageMem(hCam, pMem, memID);
  printf("Status SetImageMem %d\n",nRet);

  //Bilder im Kameraspeicher belassen
  INT displayMode = IS_SET_DM_DIB;
  nRet = is_SetDisplayMode (hCam, displayMode);
  printf("Status displayMode %d\n",nRet);

  //putting in auto exposure
  double dEnable = 1;
  nRet = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0 );
  printf("Status AutoExposure %d\n",nRet);
  //IS_SET_ENABLE_AUTO_SENSOR_SHUTTER

  //putting in auto gain
  double dEnable2 = 1;
  nRet = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &dEnable2, 0 );
  printf("Status AutoGain %d\n",nRet);

  //setting to external Hardware Trigger mode with Rising Signal Edge
  is_SetExternalTrigger(hCam, IS_SET_TRIGGER_HI_LO);
  printf("Status is_SetExternalTrigger %d\n",nRet);
  
  is_EnableEvent(hCam, IS_SET_EVENT_FRAME);
  
  //continous capture and transfer of images to image memory
  nRet = is_CaptureVideo(hCam, IS_DONT_WAIT);
  printf("Status is_CaptureVideo %d\n",nRet);
}

void checkImgMem(HIDS hCam, wchar_t* fileName) {
  Time t1(boost::posix_time::microsec_clock::local_time());
  INT nRet = is_WaitEvent(hCam, IS_SET_EVENT_FRAME, 50000);
  string retStr = WaitEventRetStr(nRet);
  //cout<< "is_WaitEvent: "<< retStr << endl;
  
  if (nRet == IS_SUCCESS) {
    /* event signalled */
    IMAGE_FILE_PARAMS ImageFileParams;
    ImageFileParams.pwchFileName = fileName;
    ImageFileParams.pnImageID = NULL;
    ImageFileParams.ppcImageMem = NULL;
    ImageFileParams.nQuality = 0;
    ImageFileParams.nFileType = IS_IMG_BMP; //JPG BMP PNG
    INT nRet = is_ImageFile(hCam, IS_IMAGE_FILE_CMD_SAVE, (void*) &ImageFileParams, sizeof(ImageFileParams));
    cout << "Status (0 is ok): " << nRet;
  }
  Time t2(boost::posix_time::microsec_clock::local_time());
  TimeDuration dt1 = t2 - t1;
  long msec1 = dt1.total_milliseconds(); 
  cout << ": Time Elapsed (ms): " << msec1 << endl; 
}

int main() {

  string countTxt;
  std::ifstream myfile("count.txt");
  myfile >> countTxt;
  cout << "txt count read is " << countTxt << endl;
  int count = boost::lexical_cast<int>(countTxt);
  cout << "converted to int is " << count << endl;
  myfile.close();
  	
  HIDS hCam1 = 1;
  HIDS hCam2 = 2;
  HIDS hCam3 = 3;
  HIDS hCam4 = 4;
  HIDS hCam5 = 5;
  HIDS hCam6 = 6;
  
  setupCamFn(hCam1);
  printf("cam1 params set\n");
  setupCamFn(hCam2);
  printf("cam2 params set\n");
  setupCamFn(hCam3);
  printf("cam3 params set\n");
  setupCamFn(hCam4);
  printf("cam4 params set\n");
  setupCamFn(hCam5);
  printf("cam5 params set\n");
  setupCamFn(hCam6);
  printf("cam6 params set\n");

  //sleep(1);
  
  string imgNoStr = "00000000";
  string str1 = "./img/C1I" + imgNoStr + ".bmp";
  string str2 = "./img/C2I" + imgNoStr + ".bmp";
  string str3 = "./img/C3I" + imgNoStr + ".bmp";
  string str4 = "./img/C4I" + imgNoStr + ".bmp";
  string str5 = "./img/C5I" + imgNoStr + ".bmp";
  string str6 = "./img/C6I" + imgNoStr + ".bmp";
  wchar_t* widecstr1 = new wchar_t[str1.length()];
  wchar_t* widecstr2 = new wchar_t[str2.length()];
  wchar_t* widecstr3 = new wchar_t[str3.length()];
  wchar_t* widecstr4 = new wchar_t[str4.length()];
  wchar_t* widecstr5 = new wchar_t[str5.length()];
  wchar_t* widecstr6 = new wchar_t[str6.length()];
  for(int i=0; i<str1.length(); i++) {
    widecstr1[i] = str1[i];
    widecstr2[i] = str2[i];
    widecstr3[i] = str3[i];
    widecstr4[i] = str4[i];
    widecstr5[i] = str5[i];
    widecstr6[i] = str6[i];
  }

  int saveCount = 0;
  
  for(int imgNo = count; imgNo <= 99999999; imgNo++) {
    if(imgNo < 10)
      imgNoStr = "0000000" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 100)
      imgNoStr = "000000" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 1000)
      imgNoStr = "00000" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 10000)
      imgNoStr = "0000" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 100000)
      imgNoStr = "000" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 1000000)
      imgNoStr = "00" + boost::lexical_cast<string>(imgNo);
    else if(imgNo < 10000000)
      imgNoStr = "0" + boost::lexical_cast<string>(imgNo);
    else
      imgNoStr = boost::lexical_cast<string>(imgNo);
    cout << "***** Img " + imgNoStr + " *****" << endl;

    str1 = "./img/C1I" + imgNoStr + ".bmp";
    str2 = "./img/C2I" + imgNoStr + ".bmp";
    str3 = "./img/C3I" + imgNoStr + ".bmp";
    str4 = "./img/C4I" + imgNoStr + ".bmp";
    str5 = "./img/C5I" + imgNoStr + ".bmp";
    str6 = "./img/C6I" + imgNoStr + ".bmp";

    for(int i=0; i<str1.length(); i++) {
      widecstr1[i] = str1[i];
      widecstr2[i] = str2[i];
      widecstr3[i] = str3[i];
      widecstr4[i] = str4[i];
      widecstr5[i] = str5[i];
      widecstr6[i] = str6[i];
    }
    
    checkImgMem(hCam1, widecstr1);
    checkImgMem(hCam2, widecstr2);
    checkImgMem(hCam3, widecstr3);
    checkImgMem(hCam4, widecstr4);
    checkImgMem(hCam5, widecstr5);
    checkImgMem(hCam6, widecstr6);
    
    saveCount++;
    if(saveCount == 10) {
      ofstream myfile;
      myfile.open("count.txt", ios::out | ios::trunc);
      myfile << imgNo;
      myfile.close();
      saveCount = 0;
    }
  }
  

  //Kamera wieder freigeben
  printf("Exit cam1\n");
  is_DisableEvent(hCam1, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam1);
  
  printf("Exit cam2\n");
  is_DisableEvent(hCam2, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam2);
  
  printf("Exit cam3\n");
  is_DisableEvent(hCam3, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam3);
  
  printf("Exit cam4\n");
  is_DisableEvent(hCam4, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam4);
  
  printf("Exit cam5\n");
  is_DisableEvent(hCam5, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam5);
  
  printf("Exit cam6\n");
  is_DisableEvent(hCam6, IS_SET_EVENT_FRAME);
  is_ExitCamera(hCam6);
  
  return 0;
}

// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/

#ifndef _WSOCR_H_
#define _WSOCR_H_

#include <string>
#include <staff/common/IService.h>



  //! wsOCR1 service
  class wsOCR: public staff::IService
  {
  public:
// *restEnable: true
// *restMethod: GET
// *restLocation: sendClientID/clientID={clientID}
	virtual double sendClientID(double clientID) = 0;

	    virtual std::string receiveContent(int linker, int flag) = 0;

		
	virtual void sendContent(std::string content, int linker, int flag) = 0;
	virtual void sendFile(std::string fileContent,std::string fileAddress) = 0;
	virtual std::string sendFile(std::string fileContent) = 0; //returns fileAddress;
	virtual int sendFileClient(std::string fileContent) = 0; //returns filenumber;
	virtual std::string receiveFile(std::string fileAddress) = 0;

	virtual void sendFileInfo(std::string filename,std::string fileInfo) = 0;
	virtual std::string receiveFileInfo(std::string filename) = 0;


    virtual void doOCR(std::string fileAddress) = 0;
	virtual void doOCR(double clientID) = 0;
// *restEnable: true
// *restMethod: GET
// *restLocation: receiveOCR/fileAddress={fileAddress}
    virtual std::string receiveOCR(std::string fileAddress) = 0;

	virtual std::string perform(std::string AppData) = 0;




  };


#endif // _WSOCR_H_


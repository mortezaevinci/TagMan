// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#ifndef _wsOCRImpl_h_
#define _wsOCRImpl_h_

#include "wsOCR.h"


  //! implementation of wsOCR1 service
  class wsOCRImpl: public wsOCR
  {
  public:
    wsOCRImpl();
    virtual ~wsOCRImpl();
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual double sendClientID(double clientID);
    virtual std::string receiveContent(int linker, int flag);
    virtual void sendContent(std::string content, int linker, int flag);
    virtual void sendFile(std::string fileContent, std::string fileAddress);
    virtual std::string sendFile(std::string fileContent);
    virtual int sendFileClient(std::string fileContent);
    virtual std::string receiveFile(std::string fileAddress);
    virtual void sendFileInfo(std::string filename, std::string fileInfo);
    virtual std::string receiveFileInfo(std::string filename);
    virtual void doOCR(std::string fileAddress);
    virtual void doOCR(double clientID);
    virtual std::string receiveOCR(std::string fileAddress);
    virtual std::string perform(std::string AppData);
  };


#endif // _wsOCRImpl_h_

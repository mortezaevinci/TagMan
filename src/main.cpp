// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <memory>
#include <staff/utils/Log.h>
#include <staff/common/logoperators.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include "wsOCR.h"
#include <iostream>
#include "myComm.h"
#include "fileFunctions.h"

#include <cstdlib> 
#include <libconfig.h++>

//#include <rise/common/DataEncoder.h>
//#include <rise/common/StreamBuffer.h>

using namespace std;
using namespace libconfig;

int main(int argc, char** argv)
{
	
	cout<< "hellO!";

		string tdoOCRResult;


	Config cfg;


	
	 try
  {
    cfg.readFile("client.cfg");
  }
  catch(const FileIOException &fioex)
  {
    std::cerr << "I/O error while reading file." << std::endl;
    return(EXIT_FAILURE);
  }
  catch(const ParseException &pex)
  {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    return(EXIT_FAILURE);
  }

    const Setting& root = cfg.getRoot();

	const Setting& clientSetting=root["clientSetting"];



  try
  {
	  //"http://54.225.96.208:9090/axis2/services/wsOCR"
	  string serviceURL;
	  clientSetting.lookupValue("serviceURL",serviceURL);
	    std::auto_ptr< ::wsOCR > pwsOCR(::staff::ServiceFactory::Inst().GetService< ::wsOCR >(serviceURL));

    STAFF_ASSERT(pwsOCR.get(), "Cannot get client for service wsOCR!");


	 string fileAddressClient;
	 string fileAddressServer;

		 //="q:\\code\\Tesseract-OCR_setup\\ocrtest.jpg";
	 clientSetting.lookupValue("fileAddressServer",fileAddressServer);
	 clientSetting.lookupValue("fileAddressClient",fileAddressClient);
	 string fileContent;
	 
	 fileContent=file2str(fileAddressClient);

	 //const char* pBuffIn=fileContent.c_str();// - input buffer (const char*)
	 //unsigned long int nBuffInSize=sizeof(pBuffIn); // nBuffInSize - buffer size (unsigned)
     //string fileContentEncoded; // sOut - resulting buffer, encoded to base64 (std::string)
     //rise::CBase64Encoder::Encode(reinterpret_cast<const rise::TBuffer*>(pBuffIn), nBuffInSize, fileContentEncoded);



	 string fileContentEncoded=str2hex(fileContent);



     AppData AppDatac;
	 AppDatac.clientID="001";
	 AppDatac.clientKey=generateKey(AppDatac.clientID);
	 AppDatac.file=fileContent;
	 AppDatac.filenameBase="Base";
	 AppDatac.filenameRelative="0001";
	 AppDatac.filenameExt="jpg";
	 string AppDataStr=AppDatac.Serialize();

	 //pwsOCR->sendFile(fileContentEncoded,fileAddressServer);
	 
     //tdoOCRResult = pwsOCR->receiveOCR(fileAddressServer);


     const std::string& tperformResult = pwsOCR->perform(AppDataStr);
    // staff::LogInfo() << "perform result: " << tperformResult; 

	 tdoOCRResult=hex2str(tperformResult);



    // Invoke Your service here:

    // double clientID = 0;
    // double tsendClientIDResult = pwsOCR->sendClientID(clientID);
    // staff::LogInfo() << "sendClientID result: " << tsendClientIDResult;

    // int linker = 0;
    // int flag = 0;
    // const std::string& treceiveContentResult = pwsOCR->receiveContent(linker, flag);
    // staff::LogInfo() << "receiveContent result: " << treceiveContentResult;

    // std::string content;
    // int linker = 0;
    // int flag = 0;
    // pwsOCR->sendContent(content, linker, flag);
    // staff::LogInfo() << "sendContent called";

    // std::string fileContent;
    // std::string fileAddress;
    // pwsOCR->sendFile(fileContent, fileAddress);
    // staff::LogInfo() << "sendFile called";

    // std::string fileContent;
    // const std::string& tsendFileResult = pwsOCR->sendFile(fileContent);
    // staff::LogInfo() << "sendFile result: " << tsendFileResult;

    // std::string fileContent;
    // int tsendFileClientResult = pwsOCR->sendFileClient(fileContent);
    // staff::LogInfo() << "sendFileClient result: " << tsendFileClientResult;

    // std::string fileAddress;
    // const std::string& treceiveFileResult = pwsOCR->receiveFile(fileAddress);
    // staff::LogInfo() << "receiveFile result: " << treceiveFileResult;

    // std::string filename;
    // std::string fileInfo;
    // pwsOCR->sendFileInfo(filename, fileInfo);
    // staff::LogInfo() << "sendFileInfo called";

    // std::string filename;
    // const std::string& treceiveFileInfoResult = pwsOCR->receiveFileInfo(filename);
    // staff::LogInfo() << "receiveFileInfo result: " << treceiveFileInfoResult;

    // std::string fileAddress;
    // pwsOCR->doOCR(fileAddress);
    // staff::LogInfo() << "doOCR called";

    // double clientID = 0;
    // pwsOCR->doOCR(clientID);
    // staff::LogInfo() << "doOCR called";

    // std::string fileAddress;
    // const std::string& treceiveOCRResult = pwsOCR->receiveOCR(fileAddress);
    // staff::LogInfo() << "receiveOCR result: " << treceiveOCRResult;

  }

  STAFF_CATCH_ALL

	  	  	 // cout<<tdoOCRResult;
		 cout<<"done!"<<endl;
          cout<<tdoOCRResult.c_str();

	  char cc;
  cin>>cc;


  return 0;
}


#include <string.h>
#include <sstream>
#include "strHex.h"
#include "sqlite3.h"

#define iferr if(error){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));};

struct AppData
{
	
	string clientID;
	string clientKey;

	//address= fileaddress+"\\"+filenameBase+filenameRelative+"."+filenameExt
	string fileAddress;
	string filenameBase;
	string filenameRelative;
	string filenameExt;

	string file;

    std::string Serialize() 
  {
  std::ostringstream out;
  string version = "1";
  out << version << endl<< clientID << endl << clientKey << endl <<str2hex(fileAddress)<<endl<<
	  str2hex(filenameBase)<<endl<<str2hex(filenameRelative)<<endl<<str2hex(filenameExt)<<endl<<str2hex(file);
  return out.str();
}

  void Deserialize(const std::string& iString)
{
  std::istringstream in(iString);
  string version = "0";
  getline(in, version);
 

   if (version=="1")
  {
	  string temp;

   getline(in,clientID);
   getline(in,clientKey);

   getline(in,temp);
   fileAddress=hex2str(temp);
   getline(in,temp);
   filenameBase=hex2str(temp);
   getline(in,temp);
   filenameRelative=hex2str(temp);
   getline(in,temp);
   filenameExt=hex2str(temp);
   getline(in,temp);
   file=hex2str(temp);
   }
   else
   {
    // Handle
  }

}
};

 
string generateKey(string clientID)
{
	//generate a key for security
	//TODO

	string Key;

	Key=str2hex(clientID);

	return Key;
}
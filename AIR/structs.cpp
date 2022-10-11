#include <fstream>
#include "structs.hpp"

structs::structs()
{
  cfh = new CentralFileHeader;
  dd = new dataDescriptor;
  lfh = new localFileHeader;
  eocd = new endOfCentralDirectory;
}

structs::~structs(){
  delete cfh;
  delete dd;
  delete lfh;
  delete eocd;
}

void structs::writeDD(std::ofstream file){
  file.write((char*)dd->CRC_32,sizeof(dd->CRC_32));
  file.write((char*)dd->compressSize,sizeof(dd->compressSize));
  file.write((char*)dd->unCompressSize,sizeof(dd->unCompressSize));
}

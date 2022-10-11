#include "toZipData.hpp"
#include <iostream>
#include <fstream>

data::data()
{
  fileZip = std::ofstream(pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::in);
}

void data::writeHeader(QString header){
  if(header.size()==0){
    std::cout << "header.size==0(toZipData.cpp)" << std::endl;
    exit(0);
  }
  char config = (header.toStdString())[0];
  fileZip = std::ofstream(pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::in);

  switch(config){
  case 'c':
    writeCFH(fileZip);
    break;
  case 'd':
    writeDD(fileZip);
    break;
  case 'l':
    writeLFH(fileZip);
    break;
  case 'e':
    writeEOCD(fileZip);
    break;
  }
}

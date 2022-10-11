#include "toZipData.hpp"
#include <iostream>
#include <fstream>

data::data()
{

}

void data::writeHeader(QString header, std::ofstream file){
  if(header.size()==0){
    std::cout << "header.size==0(toZipData.cpp)" << std::endl;
    exit(0);
  }
  char config = (header.toStdString())[0];
  switch(config){
  case 'c':
    writeCFH(fileZip);
    break;
  case 'd':
    writeDD(file);
    break;
  case 'l':
    writeLFH(file);
    break;
  case 'e':
    writeEOCD(file);
    break;
  }
}

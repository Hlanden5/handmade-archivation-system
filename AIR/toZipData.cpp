#include "toZipData.hpp"

data::data()
{
  fileZip = std::ofstream(pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::in);
}

data::~data(){
  delete[] headerArray;
}

void data::writeHeader(QString header){
  if(header.size()==0){
    std::cout << "header.size==0(toZipData.cpp)" << std::endl;
    exit(0);
  }
  char config = (header.toStdString())[0];

  switch(config){
  case 'c':
    writeCFH(fileZip);
    break;
  /*case 'd':
    writeDD(fileZip);
    break;*/
  case 'l':
    writeLFH(fileZip);
    break;
  case 'e':
    writeEOCD(fileZip);
    break;
  }
}

void data::setPathOfFiles(std::vector<QString> pathOfFiles){
  std::vector<QString>::iterator iter_1 = pathOfFiles.begin();
  for(;iter_1!=pathOfFiles.end();iter_1++)
    this->pathOfFiles.push_back(*iter_1);
  headerArray = new structs[pathOfFiles.size()];
}

void data::setPathOfZip(QString pathOfZip){
  this->pathOfZip = pathOfZip;
}


#include "crc.hpp"
#include "structs.hpp"
#include "tounzipdata.hpp"
#include <cstddef>
#include <ios>
#include <iostream>
#include <string>

toUnzipData::toUnzipData()
{

}

toUnzipData::~toUnzipData()
{

}

size_t toUnzipData::findEOCD(){ // return first byte of eocd.eocd_signature
  getDataFromZip();
  size_t pos = data.find_last_of(eocd.eocd_signature);
  if(!(data.size()-pos<30 and pos!=-1))
    throw;
  return pos;
}

void toUnzipData::setData(QString &pathOfZip,QString &pathOfFiles){
  this->pathOfZip = pathOfZip;
  this->pathOfFiles = pathOfFiles;
  this->fileZip = std::ifstream(pathOfZip.toLocal8Bit(),std::ios_base::binary);
  if(!fileZip.is_open())
    throw;
}

void toUnzipData::getDataFromZip(){
  fileZip.seekg(0,std::ios::end);
  size_t len_input = fileZip.tellg();
  fileZip.seekg(0,std::ios::beg);
  data.resize(len_input);
  fileZip.read((char*)&data[0],len_input);
  fileZip.close();
}

void toUnzipData::setEOCD(){
  size_t posEOCD = findEOCD();
  std::cout << posEOCD << std::endl;
}

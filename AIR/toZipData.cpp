#include "toZipData.hpp"
#include "crc.hpp"

data_::data_()
{
}

data_::~data_()
{
}

void data_::writeHeader(QString header){
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

void data_::setPathOfFiles(std::vector<QString> pathOfFiles){
  std::vector<QString>::iterator iter_1 = pathOfFiles.begin();
  for(;iter_1!=pathOfFiles.end();iter_1++)
    this->pathOfFiles.push_back(*iter_1);
  headerArray.resize(this->pathOfFiles.size());
}

void data_::setPathOfZip(QString pathOfZip){
  this->pathOfZip = pathOfZip;
  fileZip = std::ofstream(pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::out);
}

void data_::collectAndLoadData(){ // tmp realisation, needs repair
  std::vector<structs>::iterator iter = headerArray.begin();
  for(size_t i=0;i<pathOfFiles.size();i++,iter++){
    /*if(directory)*/
    iter->lfh->neededVersion = 1.0;
    iter->lfh->flag = 0;
    iter->lfh->methodOfCompress = 0;
    iter->lfh->timeOfLastEdit = 0;// needs further implementation
    iter->lfh->dataOfLastEdit = 0;// needs further implementation
    QByteArray data;
    size_t size;
    iter->lfh->CRC_32_uncompress = crc32File(pathOfFiles[i].toStdString(),size,data);
    iter->lfh->compressSize = size;// needs further implementation
    iter->lfh->nonCompressSize = size;
    iter->lfh->sizeofNameFile = pathOfFiles[i].toStdString().size(); // needs parsing
    iter->lfh->additionalSizeof = 0; // needs further implementation
    iter->lfh->nameOfFile = "pathOfFiles[i]";

    iter->cfh->neededVersion = iter->lfh->neededVersion;
    iter->cfh->flag = iter->lfh->flag;
    iter->cfh->methodOfCompress = iter->lfh->methodOfCompress;
    iter->cfh->timeOfLastEdit = iter->lfh->timeOfLastEdit;
    iter->cfh->dataOfLastEdit = iter->lfh->dataOfLastEdit;
    iter->cfh->CRC_32_uncompress = iter->lfh->CRC_32_uncompress;
    iter->cfh->compressSize = iter->lfh->compressSize;
    iter->cfh->nonCompressSize = iter->lfh->nonCompressSize;
    iter->cfh->sizeofNameFile = iter->lfh->sizeofNameFile;
    iter->cfh->additionalSizeof = iter->lfh->additionalSizeof;
    iter->cfh->nameOfFile = iter->lfh->nameOfFile;
    iter->cfh->sizeofComment = 0; // needs further implementation
    iter->cfh->numberOfDrive = 0; // maybe needs further implementation
    iter->cfh->internalAttributes = 0; // needs further implementation
    iter->cfh->externalAttributes = 0;// needs further implementation
    iter->cfh->offset = 0; // needs further implementation
    iter->cfh->comment = pathOfFiles[i]; // needs further implementation

    iter->eocd->numberOfDrive = iter->cfh->numberOfDrive;
    iter->eocd->numberOfDriveCFH = 0; // needs further implementation
    iter->eocd->countOfCFH_onThisDrive = 0; // needs further implementation
    iter->eocd->countOfCFH = 0; // needs further implementation
    iter->eocd->sizeofCFH = sizeof(cfh)*iter->eocd->countOfCFH; // needs further implementation
    iter->eocd->offsetCFH_ofStartArchive = 0; // needs further implementation
    iter->eocd->sizeofComment = 0; // needs further implementation
    iter->eocd->comment = pathOfFiles[i]; // needs further implementation

    iter->writeLFH(fileZip);
    {
      std::string x = QString(data).toStdString();
      fileZip.write((char*)x.c_str(),x.size());
    }
    iter->writeCFH(fileZip);
    iter->writeEOCD(fileZip);
  }
  fileZip.close();
}


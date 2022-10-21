#include "toZipData.hpp"
#include "crc.hpp"

data_::data_()
{
}

data_::~data_()
{
}

void data_::setPathOfFiles(std::vector<QString> pathOfFiles){
  std::vector<QString>::iterator iter_1 = pathOfFiles.begin();
  for(;iter_1!=pathOfFiles.end();iter_1++)
    this->pathOfFiles.push_back(*iter_1);
  headerArray.resize(this->pathOfFiles.size());
}

std::string getNameOfFile(std::string path){
  std::string tmp;
  tmp.swap(path);
  tmp = tmp.substr(path.find_last_of("/\\")+1);
  return tmp;
}

void data_::setPathOfZip(QString pathOfZip){
  this->pathOfZip = pathOfZip;
  std::string tmp(pathOfFiles[0].toStdString());
  tmp = tmp.substr(tmp.find_last_of("/\\")+1);
  this->pathOfZip += QString::fromStdString(tmp);
  this->pathOfZip += ".zip";
  fileZip = std::ofstream(pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if(!fileZip.is_open())
    throw;
}

void data_::collectAndLoadData(){ // tmp realisation, needs repair
  std::vector<structs>::iterator iter = headerArray.begin();
  for(size_t i=0;i<pathOfFiles.size();i++,iter++){
    /*if(directory)*/
    iter->lfh->neededVersion = 0x0A00;
    iter->lfh->flag = 0;
    iter->lfh->methodOfCompress = 0;
    iter->lfh->timeOfLastEdit = 0x9752;// needs further implementation
    iter->lfh->dataOfLastEdit = 0x4A55;// needs further implementation
    QByteArray data;
    size_t size;
    iter->lfh->CRC_32_uncompress = crc32File(pathOfFiles[i].toStdString(),size,data);
    iter->lfh->compressSize = size;// needs further implementation
    iter->lfh->nonCompressSize = size;
    iter->lfh->additionalSizeof = 0; // needs further implementation
    std::string tmp(pathOfFiles[i].toStdString());
    tmp = tmp.substr(tmp.find_last_of("/\\")+1);
    iter->lfh->nameOfFile.swap(tmp);
    iter->lfh->sizeofNameFile = iter->lfh->nameOfFile.size();

    iter->cfh->versionDone = 0x0000;
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
    //iter->cfh->offset = sizeof(lfh)+iter->cfh->compressSize+iter->cfh->sizeofNameFile; // needs further implementation
    iter->cfh->offset = 0;
    iter->cfh->comment = ""; // needs further implementation

    iter->eocd->numberOfDrive = 0;//iter->cfh->numberOfDrive;
    iter->eocd->numberOfDriveCFH = 0; // needs further implementation
    iter->eocd->countOfCFH_onThisDrive = 1; // needs further implementation
    iter->eocd->countOfCFH = 1; // needs further implementation
    iter->eocd->sizeofCFH = (sizeof(CentralFileHeader)+iter->cfh->sizeofNameFile
                             +iter->cfh->sizeofComment-2*sizeof(std::string))
        *iter->eocd->countOfCFH-2; // needs further implementation
    iter->eocd->offsetCFH_ofStartArchive = sizeof(localFileHeader)-sizeof(std::string)
        +iter->cfh->compressSize+iter->cfh->sizeofNameFile; // needs further implementation
    iter->eocd->sizeofComment = 0; // needs further implementation
    iter->eocd->comment = ""; // needs further implementation

    iter->writeLFH(fileZip);
    for(const auto&c:qAsConst(data))
        fileZip << c;

    iter->eocd->offsetCFH_ofStartArchive = fileZip.tellp();
    iter->writeCFH(fileZip);
//    fileZip << q0x05054b50;
//    fileZip << data.size()/8;
    iter->writeEOCD(fileZip);
  }
  fileZip.close();
}


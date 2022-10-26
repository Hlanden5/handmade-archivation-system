#include "toZipData.hpp"
#include "crc.hpp"

data_::data_()
{
}

data_::~data_()
{
}

void data_::setPathOfFiles(QString pathOfFiles){
  filesMetadata tmp;
  tmp.setPath(pathOfFiles);
  tmp.collectData();
  //tmp.printMetadata();//test
  metainfo.swap(tmp.getMetadata());
  headerArray.resize(metainfo.size());
  this->pathOfFiles = pathOfFiles;
}

std::string getNameOfFile(std::string path){
  std::string tmp;
  tmp.swap(path);
  tmp = tmp.substr(path.find_last_of("/\\")+1);
  return tmp;
}

void data_::setPathOfZip(QString pathOfZip){
  {
    QString tmp_2;
    std::string tmp(metainfo[0].name.toStdString());
    tmp_2.push_back(QString::fromStdString(tmp.substr(tmp.find_last_of("/\\")+1)));
    tmp_2.push_back(".zip");
    tmp_2.push_front(pathOfZip);
    this->pathOfZip = tmp_2;
  }
  for(std::vector<metadata>::const_iterator c = metainfo.begin();c!=metainfo.end();c++){
      if(c->name == this->pathOfZip)
        metainfo.erase(c);
    }
  //std::remove(this->pathOfZip.toStdString().c_str());
  fileZip = std::ofstream(this->pathOfZip.toStdString(),std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if(!fileZip.is_open())
    throw;
}

void data_::delMainPath(std::string &path){
  path = path.erase(0,pathOfFiles.size()+1);
}

void data_::collectAndLoadData(){ // tmp realisation, needs repair

  std::vector<structs>::iterator iter = headerArray.begin();
  quint16 countCFH = 0;
  quint32 offsetCFH = 0;
  quint32 sizeAllCFH = 0;
  for(size_t i=0;i<metainfo.size();i++,iter++){
      clock_t t0 = clock();
      if(metainfo[i].is_directory == true){
          iter->lfh->neededVersion = 0x0A00;
          iter->lfh->flag = 0;
          iter->lfh->methodOfCompress = 0;
          iter->lfh->timeOfLastEdit = metainfo[i].timeOfLastEdit;// needs further implementation
          iter->lfh->dataOfLastEdit = metainfo[i].dataOfLastEdit;// needs further implementation
          iter->lfh->CRC_32_uncompress = 0;
          iter->lfh->compressSize = 0;// needs further implementation
          iter->lfh->nonCompressSize = 0;
          iter->lfh->additionalSizeof = 0; // needs further implementation
          std::string tmp(metainfo[i].name.toStdString());
          delMainPath(tmp);
          tmp+="\\";
          iter->lfh->nameOfFile.swap(tmp);

          iter->lfh->sizeofNameFile = iter->lfh->nameOfFile.size();

          iter->cfh->offset = fileZip.tellp();
          iter->writeLFH(fileZip);

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
          iter->cfh->comment = ""; // needs further implementation

          countCFH++;
          sizeAllCFH += (sizeof(CentralFileHeader)-2+iter->cfh->sizeofNameFile+iter->cfh->sizeofComment-2*sizeof(std::string));
          if(!(i+1<metainfo.size()))
            offsetCFH = fileZip.tellp();
        }else{
          iter->lfh->neededVersion = 0x0A00;
          iter->lfh->flag = 0;
          iter->lfh->methodOfCompress = 0;
          iter->lfh->timeOfLastEdit = metainfo[i].timeOfLastEdit;// needs further implementation
          iter->lfh->dataOfLastEdit = metainfo[i].dataOfLastEdit;// needs further implementation
          QByteArray data;
          size_t size = metainfo[i].sizeofFile;
          data.resize(size);
          iter->lfh->CRC_32_uncompress = crc32File(metainfo[i].name.toStdString(),data);
//          std::cout << "File : " << metainfo[i].name.toStdString()
//                    << "\tCRC-32 " << std::hex << iter->lfh->CRC_32_uncompress
//                    << std::dec << std::endl;
          iter->lfh->compressSize = size;// needs further implementation
          iter->lfh->nonCompressSize = size;
          iter->lfh->additionalSizeof = 0; // needs further implementation
          std::string tmp(metainfo[i].name.toStdString());
          delMainPath(tmp);
          //tmp = tmp.substr(tmp.find_last_of("/\\")+1);
          iter->lfh->nameOfFile.swap(tmp);

          iter->lfh->sizeofNameFile = iter->lfh->nameOfFile.size();

          iter->cfh->offset = fileZip.tellp();
          iter->writeLFH(fileZip);

          for(const auto&c:qAsConst(data))
            fileZip << c;

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
          iter->cfh->comment = ""; // needs further implementation

          countCFH++;
          sizeAllCFH += (sizeof(CentralFileHeader)-2+iter->cfh->sizeofNameFile+iter->cfh->sizeofComment-2*sizeof(std::string));
          if(!(i+1<metainfo.size()))
            offsetCFH = fileZip.tellp();
        }
      clock_t t1 = clock();
      std::cout << "Filename:" << metainfo[i].name.toStdString() << "\ttime: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    }

  iter = headerArray.begin();
  for(size_t i=0;i<metainfo.size();i++,iter++)
    iter->writeCFH(fileZip);
  //iter = headerArray[headerArray.size()]
  //std::cout << sizeAllCFH << std::endl;
  iter = headerArray.begin();
  iter->eocd->numberOfDrive = 0;//iter->cfh->numberOfDrive;
  iter->eocd->numberOfDriveCFH = 0; // needs further implementation
  iter->eocd->countOfCFH_onThisDrive = countCFH; // needs further implementation
  iter->eocd->countOfCFH = countCFH; // needs further implementation
  iter->eocd->sizeofCFH = sizeAllCFH; // needs further implementation
  iter->eocd->offsetCFH_ofStartArchive = offsetCFH;
  iter->eocd->sizeofComment = 0; // needs further implementation
  iter->eocd->comment = ""; // needs further implementation

  iter->writeEOCD(fileZip);
  fileZip.close();
}


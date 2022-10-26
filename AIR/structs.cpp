#include "structs.hpp"

#define LE(x) (qFromLittleEndian(x))
#define BE(x) (qFromBigEndian(x))

structs::structs()
{
  cfh = new CentralFileHeader;
  //dd = new dataDescriptor;
  lfh = new localFileHeader;
  eocd = new endOfCentralDirectory;
}

structs::~structs(){
  delete cfh;
  //delete dd;
  delete lfh;
  delete eocd;
}

void structs::writeCFH(std::ofstream &file){
  QByteArray dataHeader;
  QDataStream stream(&dataHeader, QIODevice::WriteOnly);
  stream << BE(cfh->cfh_signature);
  stream << LE(cfh->versionDone);
  stream << LE(cfh->neededVersion);
  stream << LE(cfh->flag);
  stream << LE(cfh->methodOfCompress);
  stream << BE(lfh->dataOfLastEdit);
  stream << BE(lfh->timeOfLastEdit);
  stream << BE(cfh->CRC_32_uncompress);
  stream << BE(cfh->compressSize);
  stream << BE(cfh->nonCompressSize);
  stream << BE(cfh->sizeofNameFile);
  stream << LE(cfh->additionalSizeof);
  stream << LE(cfh->sizeofComment);
  stream << LE(cfh->numberOfDrive);
  stream << LE(cfh->internalAttributes);
  stream << BE(cfh->externalAttributes);
  stream << BE(cfh->offset);
  dataHeader += cfh->nameOfFile;
  dataHeader += cfh->comment;
  for(const auto &c:qAsConst(dataHeader))
    file << c;
}

void structs::writeLFH(std::ofstream &file){
  QByteArray dataHeader;
  QDataStream stream(&dataHeader, QIODevice::WriteOnly);
  stream << qFromBigEndian(lfh->lfh_signature);
  stream << LE(lfh->neededVersion);
  stream << LE(lfh->flag);
  stream << LE(lfh->methodOfCompress); 
  stream << BE(lfh->dataOfLastEdit);
  stream << BE(lfh->timeOfLastEdit);
  stream << BE(lfh->CRC_32_uncompress);
  stream << BE(lfh->compressSize);
  stream << BE(lfh->nonCompressSize);
  stream << BE(lfh->sizeofNameFile);
  stream << LE(lfh->additionalSizeof);
  dataHeader += lfh->nameOfFile;
  for(const auto &c:qAsConst(dataHeader))
    file << c;
}

void structs::writeEOCD(std::ofstream &file){
  QByteArray dataHeader;
  QDataStream stream(&dataHeader, QIODevice::WriteOnly);
  stream << qFromBigEndian(eocd->eocd_signature);
  stream << LE(eocd->numberOfDrive);
  stream << LE(eocd->numberOfDriveCFH);
  stream << BE(eocd->countOfCFH_onThisDrive);
  stream << BE(eocd->countOfCFH);
  stream << BE(eocd->sizeofCFH);
  stream << BE(eocd->offsetCFH_ofStartArchive);
  stream << LE(eocd->sizeofComment);
  dataHeader += eocd->comment;
  for(const auto &c:qAsConst(dataHeader))
    file << c;
}

/*void structs::writeDD(std::ofstream &file){
  file.write((char*)dd->CRC_32,sizeof(dd->CRC_32));
  file.write((char*)dd->compressSize,sizeof(dd->compressSize));
  file.write((char*)dd->unCompressSize,sizeof(dd->unCompressSize));
}*/

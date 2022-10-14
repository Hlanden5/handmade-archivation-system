#include "structs.hpp"

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
  file.write((char*)&cfh->cfh_signature,sizeof(cfh->cfh_signature));
  file.write((char*)&cfh->versionDone,sizeof(cfh->versionDone));
  file.write((char*)&cfh->neededVersion,sizeof(cfh->neededVersion));
  file.write((char*)&cfh->flag,sizeof(cfh->flag));
  file.write((char*)&cfh->methodOfCompress,sizeof(cfh->methodOfCompress));
  file.write((char*)&cfh->timeOfLastEdit,sizeof(cfh->timeOfLastEdit));
  file.write((char*)&cfh->dataOfLastEdit,sizeof(cfh->dataOfLastEdit));
  file.write((char*)&cfh->CRC_32_uncompress,sizeof(cfh->CRC_32_uncompress));
  file.write((char*)&cfh->compressSize,sizeof(cfh->compressSize));
  file.write((char*)&cfh->nonCompressSize,sizeof(cfh->nonCompressSize));
  file.write((char*)&cfh->sizeofNameFile,sizeof(cfh->sizeofNameFile));
  file.write((char*)&cfh->additionalSizeof,sizeof(cfh->additionalSizeof));
  file.write((char*)&cfh->sizeofComment,sizeof(cfh->sizeofComment));
  file.write((char*)&cfh->numberOfDrive,sizeof(cfh->numberOfDrive));
  file.write((char*)&cfh->internalAttributes,sizeof(cfh->internalAttributes));
  file.write((char*)&cfh->externalAttributes,sizeof(cfh->externalAttributes));
  file.write((char*)&cfh->offset,sizeof(cfh->offset));
  file.write((char*)&cfh->nameOfFile,sizeof(cfh->nameOfFile));
  file.write((char*)&cfh->comment,sizeof(cfh->comment));
}

void structs::writeLFH(std::ofstream &file){
  file.write((char*)&lfh->lfh_signature,sizeof(lfh->lfh_signature));
  file.write((char*)&lfh->neededVersion,sizeof(lfh->neededVersion));
  file.write((char*)&lfh->flag,sizeof(lfh->flag));
  file.write((char*)&lfh->methodOfCompress,sizeof(lfh->methodOfCompress));
  file.write((char*)&lfh->timeOfLastEdit,sizeof(lfh->timeOfLastEdit));
  file.write((char*)&lfh->dataOfLastEdit,sizeof(lfh->dataOfLastEdit));
  file.write((char*)&lfh->CRC_32_uncompress,sizeof(lfh->CRC_32_uncompress));
  file.write((char*)&lfh->compressSize,sizeof(lfh->compressSize));
  file.write((char*)&lfh->nonCompressSize,sizeof(lfh->nonCompressSize));
  file.write((char*)&lfh->sizeofNameFile,sizeof(lfh->sizeofNameFile));
  file.write((char*)&lfh->additionalSizeof,sizeof(lfh->additionalSizeof));
  file.write((char*)&lfh->nameOfFile,sizeof(lfh->nameOfFile));
}

void structs::writeEOCD(std::ofstream &file){
  file.write((char*)&eocd->eocd_signature,sizeof(eocd->eocd_signature));
  file.write((char*)&eocd->numberOfDrive,sizeof(eocd->numberOfDrive));
  file.write((char*)&eocd->numberOfDriveCFH,sizeof(eocd->numberOfDriveCFH));
  file.write((char*)&eocd->countOfCFH_onThisDrive,sizeof(eocd->countOfCFH_onThisDrive));
  file.write((char*)&eocd->countOfCFH,sizeof(eocd->countOfCFH));
  file.write((char*)&eocd->sizeofCFH,sizeof(eocd->sizeofCFH));
  file.write((char*)&eocd->offsetCFH_ofStartArchive,sizeof(eocd->offsetCFH_ofStartArchive));
  file.write((char*)&eocd->sizeofComment,sizeof(eocd->sizeofComment));
  file.write((char*)&eocd->comment,sizeof(eocd->comment));
}

/*void structs::writeDD(std::ofstream &file){
  file.write((char*)dd->CRC_32,sizeof(dd->CRC_32));
  file.write((char*)dd->compressSize,sizeof(dd->compressSize));
  file.write((char*)dd->unCompressSize,sizeof(dd->unCompressSize));
}*/

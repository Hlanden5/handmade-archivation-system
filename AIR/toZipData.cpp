#include "toZipData.hpp"
#include "crc.hpp"

data_::data_()
{
}

data_::~data_()
{
}

bool data_::isUtf_8(std::string toCheck){
  std::string tmp("ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿ");
  for(const auto &c:toCheck){
      if(tmp.find(c)!=std::string::npos)
        return true;
    }
  return false;
}

void data_::setPathOfFiles(QString pathOfFiles){
  filesMetadata tmp;
  tmp.setPath(pathOfFiles);
  tmp.collectData();
  tmp.printMetadata();//test
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

std::string data_::delMainPath(std::string path){
  return path.erase(0,pathOfFiles.size());
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
          //          if(isUtf_8(metainfo[i].name.toLocal8Bit().toStdString())){
          //              QString tmp;
          //              tmp+=delMainPath(metainfo[i].name.toLocal8Bit().toStdString()) + "\\";
          //              tmp = tmp.toUtf8();
          //              for(const auto &c:qAsConst(tmp)){
          //                  std::bitset<8> x(tmp[0].toLatin1());
          //                  iter->lfh->nameOfFile += char(x.to_ulong());
          //                }
          //            }
          //          else{

          iter->lfh->nameOfFile = delMainPath(metainfo[i].name.toLocal8Bit().toStdString()) + "\\";
          //}
          //std::string tmp(metainfo[i].name.toLocal8Bit());
          //iter->lfh->nameOfFile = delMainPath(metainfo[i].name.toLocal8Bit().toStdString()) + "\\";
          iter->lfh->sizeofNameFile = iter->lfh->nameOfFile.size();
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
          //std::string tmp(metainfo[i].name.toUtf8());
          std::string tmp(metainfo[i].name.toLocal8Bit());
          QByteArray data;
          size_t size = metainfo[i].sizeofFile;
          data.resize(size);
          crc32File(std::string(metainfo[i].name.toLocal8Bit()),data,iter->lfh->CRC_32_uncompress);

          //          std::cout << "File : " << metainfo[i].name.toLocal8Bit().toStdString()
          //                    << "\tCRC-32 " << std::hex << iter->lfh->CRC_32_uncompress
          //                    << std::dec << std::endl;
          iter->lfh->neededVersion = 0x0A00;

          std::bitset<sizeof(iter->lfh->flag)*8> flagBits;
          iter->lfh->flag = 0;
//          std::cout <<metainfo[i].name.toUtf8().isValidUtf8() << "\t"
//                   << metainfo[i].name.toLocal8Bit().isValidUtf8()
//                   << metainfo[i].name.isValidUtf16()
//                   << std::endl;
          if(metainfo[i].name.isValidUtf16()){ // need to check if UTF8
              flagBits.flip(11);
              iter->lfh->extraField += 0x75;
              iter->lfh->extraField += 0x70;
              iter->lfh->additionalSizeof = iter->lfh->extraField.size();
              //iter->lfh->extraField += " î’ƒ1";
//              iter->lfh->extraField += 0x75;
//              iter->lfh->extraField += 0x63;
              iter->lfh->extraField += delMainPath(std::string(metainfo[i].name.toUtf8()));
              //iter->cfh->extraField += "                     u|·øñØ)B0Î:÷Ø±ÌáöØup î’ƒ1Ð¢";
              iter->cfh->extraField += delMainPath(std::string(metainfo[i].name.toUtf8()));
              iter->cfh->additionalSizeof = iter->cfh->extraField.size();
            }
          // if()// need further implementation, check end of this file
          iter->lfh->flag = flagBits.to_ullong();
          iter->lfh->methodOfCompress = 0;
          iter->lfh->timeOfLastEdit = metainfo[i].timeOfLastEdit;// needs further implementation
          iter->lfh->dataOfLastEdit = metainfo[i].dataOfLastEdit;// needs further implementation
          iter->lfh->compressSize = size;// needs further implementation
          iter->lfh->nonCompressSize = size;
          iter->lfh->additionalSizeof = 0; // needs further implementation
          std::string nameOfFile = delMainPath(tmp);
          iter->lfh->nameOfFile.swap(nameOfFile);
          //iter->lfh->nameOfFile.swap(tmp);
          iter->lfh->sizeofNameFile = iter->lfh->nameOfFile.size();

          iter->cfh->offset = fileZip.tellp();
          iter->writeLFH(fileZip);

          fileZip.write(data.constData(),data.size());

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
      std::cout << "Filename:" << std::string(metainfo[i].name.toUtf8()) << "\ttime: " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
    }

  iter = headerArray.begin();
  for(size_t i=0;i<metainfo.size();i++,iter++)
    iter->writeCFH(fileZip);
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

/*
4.4.4 general purpose bit flag: (2 bytes)

        Bit 0: If set, indicates that the file is encrypted.

        (For Method 6 - Imploding)
        Bit 1: If the compression method used was type 6,
               Imploding, then this bit, if set, indicates
               an 8K sliding dictionary was used.  If clear,
               then a 4K sliding dictionary was used.

        Bit 2: If the compression method used was type 6,
               Imploding, then this bit, if set, indicates
               3 Shannon-Fano trees were used to encode the
               sliding dictionary output.  If clear, then 2
               Shannon-Fano trees were used.

        (For Methods 8 and 9 - Deflating)
        Bit 2  Bit 1
          0      0    Normal (-en) compression option was used.
          0      1    Maximum (-exx/-ex) compression option was used.
          1      0    Fast (-ef) compression option was used.
          1      1    Super Fast (-es) compression option was used.

        (For Method 14 - LZMA)
        Bit 1: If the compression method used was type 14,
               LZMA, then this bit, if set, indicates
               an end-of-stream (EOS) marker is used to
               mark the end of the compressed data stream.
               If clear, then an EOS marker is not present
               and the compressed data size must be known
               to extract.

        Note:  Bits 1 and 2 are undefined if the compression
               method is any other.

        Bit 3: If this bit is set, the fields crc-32, compressed
               size and uncompressed size are set to zero in the
               local header.  The correct values are put in the
               data descriptor immediately following the compressed
               data.  (Note: PKZIP version 2.04g for DOS only
               recognizes this bit for method 8 compression, newer
               versions of PKZIP recognize this bit for any
               compression method.)

        Bit 4: Reserved for use with method 8, for enhanced
               deflating.

        Bit 5: If this bit is set, this indicates that the file is
               compressed patched data.  (Note: Requires PKZIP
               version 2.70 or greater)

        Bit 6: Strong encryption.  If this bit is set, you MUST
               set the version needed to extract value to at least
               50 and you MUST also set bit 0.  If AES encryption
               is used, the version needed to extract value MUST
               be at least 51. See the section describing the Strong
               Encryption Specification for details.  Refer to the
               section in this document entitled "Incorporating PKWARE
               Proprietary Technology into Your Product" for more
               information.

        Bit 7: Currently unused.

        Bit 8: Currently unused.

        Bit 9: Currently unused.

        Bit 10: Currently unused.

        Bit 11: Language encoding flag (EFS).  If this bit is set,
                the filename and comment fields for this file
                MUST be encoded using UTF-8. (see APPENDIX D)

        Bit 12: Reserved by PKWARE for enhanced compression.

        Bit 13: Set when encrypting the Central Directory to indicate
                selected data values in the Local Header are masked to
                hide their actual values.  See the section describing
                the Strong Encryption Specification for details.  Refer
                to the section in this document entitled "Incorporating
                PKWARE Proprietary Technology into Your Product" for
                more information.

        Bit 14: Reserved by PKWARE for alternate streams.

        Bit 15: Reserved by PKWARE.
*/

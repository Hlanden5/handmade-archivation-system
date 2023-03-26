#include "toZipData.hpp"
#include "crc.hpp"
#include "structs.hpp"

toZip::toZip()
{
}

toZip::~toZip()
{
}

void toZip::setPathOfFiles(QString pathOfFiles){
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

void toZip::setPathOfZip(QString pathOfZip){
  {
    QString tmp_2;
    std::string tmp(metainfo[0].name.toLocal8Bit());
    tmp = tmp.erase(0,tmp.find_first_of("/\\")+1);
    tmp = tmp.erase(tmp.find_first_of("/\\"),tmp.size()-1);
    tmp_2.push_back(QString::fromLocal8Bit(tmp));
    tmp_2.push_back(".zip");
    tmp_2.push_front(pathOfZip);
    this->pathOfZip = tmp_2;
  }
  for(std::vector<metadata>::const_iterator c = metainfo.begin();c!=metainfo.end();c++){
      if(c->name == this->pathOfZip)
        metainfo.erase(c);
    }
  fileZip = std::ofstream(this->pathOfZip.toLocal8Bit(),std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if(!fileZip.is_open())
    throw;
}

std::string toZip::delMainPath(std::string path){
  return path.erase(0,pathOfFiles.size());
}

void toZip::collectAndLoadData(){ // tmp realisation, needs repair
  std::vector<structs>::iterator it = headerArray.begin();
  std::vector<metadata>::iterator metainfo_it = metainfo.begin();
  std::vector<metadata>::iterator metainfo_end = metainfo.end();
  quint16 countCFH = 0;
  quint32 offsetCFH = 0;
  quint32 sizeAllCFH = 0;
  clock_t t0,t1,tc1,tc2;
  for(size_t i=0;metainfo_it<metainfo_end;it++,metainfo_it++,i++){
      t0 = clock();
      if(metainfo_it->is_directory == true){
          it->lfh->neededVersion = 0x0A00;
          it->lfh->flag = 0;
          it->lfh->methodOfCompress = 0;
          it->lfh->timeOfLastEdit = metainfo_it->timeOfLastEdit;
          it->lfh->dataOfLastEdit = metainfo_it->dataOfLastEdit;
          it->lfh->CRC_32_uncompress = 0;
          it->lfh->compressSize = 0;
          it->lfh->nonCompressSize = 0;
          it->lfh->additionalSizeof = 0;
          it->lfh->nameOfFile = delMainPath(Win1251toCP866(std::string(metainfo_it->name.toLocal8Bit()))) + "\\";
          it->lfh->sizeofNameFile = it->lfh->nameOfFile.size();
          it->lfh->sizeofNameFile = it->lfh->nameOfFile.size();

          it->cfh->offset = fileZip.tellp();
          it->writeLFH(fileZip);

          it->cfh->versionDone = 0x0000;
          it->cfh->neededVersion = it->lfh->neededVersion;
          it->cfh->flag = it->lfh->flag;
          it->cfh->methodOfCompress = it->lfh->methodOfCompress;
          it->cfh->timeOfLastEdit = it->lfh->timeOfLastEdit;
          it->cfh->dataOfLastEdit = it->lfh->dataOfLastEdit;
          it->cfh->CRC_32_uncompress = it->lfh->CRC_32_uncompress;
          it->cfh->compressSize = it->lfh->compressSize;
          it->cfh->nonCompressSize = it->lfh->nonCompressSize;
          it->cfh->sizeofNameFile = it->lfh->sizeofNameFile;
          it->cfh->additionalSizeof = it->lfh->additionalSizeof;
          it->cfh->nameOfFile = it->lfh->nameOfFile;
          it->cfh->sizeofComment = 0; // needs further implementation
          it->cfh->numberOfDrive = 0; // maybe needs further implementation
          it->cfh->internalAttributes = 0; // needs further implementation
          it->cfh->externalAttributes = 0;// needs further implementation
          it->cfh->comment = ""; // needs further implementation

          countCFH++;
          sizeAllCFH += (sizeof(CentralFileHeader)-3*sizeof(std::string)+it->cfh->sizeofNameFile+it->cfh->sizeofComment);
          if(!(i+1<metainfo.size()))
            offsetCFH = fileZip.tellp();
        }else{
          tc1 = clock();
          QByteArray data;
          size_t size = metainfo_it->sizeofFile;
          data.resize(size);
          crc32File(std::string(metainfo_it->name.toLocal8Bit()),data,it->lfh->CRC_32_uncompress);

          it->lfh->neededVersion = 0x3800;
          it->lfh->flag = 0;
          it->lfh->methodOfCompress = 0;
          it->lfh->timeOfLastEdit = metainfo_it->timeOfLastEdit;// needs further implementation
          it->lfh->dataOfLastEdit = metainfo_it->dataOfLastEdit;// needs further implementation
          it->lfh->compressSize = size;// needs further implementation
          it->lfh->nonCompressSize = size;
          it->lfh->additionalSizeof = 0; // needs further implementation
          std::string nameOfFile = delMainPath(Win1251toCP866(std::string(metainfo_it->name.toLocal8Bit())));
          it->lfh->nameOfFile.swap(nameOfFile);

          it->lfh->sizeofNameFile = it->lfh->nameOfFile.size();

          it->cfh->offset = fileZip.tellp();
          it->writeLFH(fileZip);
          fileZip.write(data.constData(),data.size());


          it->cfh->versionDone = 0x0000;
          it->cfh->neededVersion = it->lfh->neededVersion;
          it->cfh->flag = it->lfh->flag;
          it->cfh->methodOfCompress = it->lfh->methodOfCompress;
          it->cfh->timeOfLastEdit = it->lfh->timeOfLastEdit;
          it->cfh->dataOfLastEdit = it->lfh->dataOfLastEdit;
          it->cfh->CRC_32_uncompress = it->lfh->CRC_32_uncompress;
          it->cfh->compressSize = it->lfh->compressSize;
          it->cfh->nonCompressSize = it->lfh->nonCompressSize;
          it->cfh->sizeofNameFile = it->lfh->sizeofNameFile;
          it->cfh->additionalSizeof = it->lfh->additionalSizeof;
          it->cfh->nameOfFile = it->lfh->nameOfFile;
          it->cfh->sizeofComment = 0; // needs further implementation
          it->cfh->numberOfDrive = 0; // maybe needs further implementation
          it->cfh->internalAttributes = 0; // needs further implementation
          it->cfh->externalAttributes = 0;// needs further implementation
          it->cfh->comment = ""; // needs further implementation

          countCFH++;
          sizeAllCFH += (sizeof(CentralFileHeader)-2+it->cfh->sizeofNameFile+it->cfh->sizeofComment-3*sizeof(std::string)+it->cfh->extraField.size());
          tc2 = clock();
          //std::cout << "time: " << (double)(tc2 - tc1) / CLOCKS_PER_SEC << "\n";
        }
      t1 = clock();
      //std::cout << "Filename:" << std::string(metainfo_it->name.toLocal8Bit()) << "\ttime: " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
    }
  struct endOfCentralDirectory eocd;
  it = headerArray.begin();
  offsetCFH = fileZip.tellp();
  tc1 = clock();
  for(size_t i=0;i<metainfo.size();i++,it++)
    it->writeCFH(fileZip);
  tc2 = clock();
  std::cout << "time: " << (double)(tc2 - tc1) / CLOCKS_PER_SEC << "\n";
  it = headerArray.begin();
  eocd.numberOfDrive = 0;//it->cfh->numberOfDrive;
  eocd.numberOfDriveCFH = 0; // needs further implementation
  eocd.countOfCFH_onThisDrive = countCFH; // needs further implementation
  eocd.countOfCFH = countCFH; // needs further implementation
  //std::cout << "CFH SIZE " << sizeAllCFH << std::endl;
  eocd.sizeofCFH = sizeAllCFH; // needs further implementation
  eocd.offsetCFH_ofStartArchive = offsetCFH;
  eocd.comment = ""; // needs further implementation
  eocd.sizeofComment = 0; // needs further implementation

  writeEOCD(fileZip,&eocd);
  fileZip.close();
  debug("D:\\OPD\\debug.txt",eocd);
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
void toZip::debug(std::string path,struct endOfCentralDirectory eocd){
  using std::endl;
  std::ofstream debugger(path, std::ofstream::trunc);
  size_t index = 0;
  for(size_t i=0;i<headerArray.size();i++){
      debugger << endl
               << "LFH with index: " << index << endl
               << "Needed version: " << headerArray[index].lfh->neededVersion << endl
               << "Compression method: " << headerArray[index].lfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[index].lfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[index].lfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[index].lfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[index].lfh->compressSize << endl
               << "Uncompressed size: " << headerArray[index].lfh->nonCompressSize << endl
               << "Name length: " << headerArray[index].lfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[index].lfh->additionalSizeof << endl
               << "Name: " << headerArray[index].lfh->nameOfFile << endl
               << "Extra field: " << headerArray[index].lfh->extraField << endl;

      debugger << endl
               << "Central File Header with index: " << index << endl
               << "Version made by: " << headerArray[index].cfh->versionDone << endl
               << "Version needed to extract: " << headerArray[index].cfh->neededVersion << endl
               << "Compression method: " << headerArray[index].cfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[index].cfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[index].cfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[index].cfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[index].cfh->compressSize << endl
               << "Uncompressed size: " << headerArray[index].cfh->nonCompressSize << endl
               << "Name length: " << headerArray[index].cfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[index].cfh->additionalSizeof << endl
               << "Comment length: " << headerArray[index].cfh->sizeofComment << endl
               << "Number of this disk: " << headerArray[index].cfh->numberOfDrive << endl
               << "Internal file attributes: " << headerArray[index].cfh->internalAttributes << endl
               << "External file attributes: " << headerArray[index].cfh->externalAttributes << endl
               << "Offset of local header: " << headerArray[index].cfh->offset << endl
               << "Name: " << headerArray[index].cfh->nameOfFile << endl
               << "Extra field: " << headerArray[index].cfh->extraField << endl
               << "Comment: " << headerArray[index].cfh->comment << endl;
    }
  debugger << endl
           << "End of Central Directory Record" << endl
           << "Number of this disk: " << eocd.numberOfDrive << endl
           << "Number of the disk with the start of the central directory: " << eocd.numberOfDriveCFH << endl
           << "Total number of entries in the central directory on this disk: " << eocd.countOfCFH_onThisDrive << endl
           << "Total number of entries in the central directory: " << eocd.countOfCFH << endl
           << "Size of the central directory: " << eocd.sizeofCFH << " bytes" << endl
           << "Offset of start of central directory: " << eocd.offsetCFH_ofStartArchive << endl
           << "Comment length: " << eocd.sizeofComment << endl
           << "Comment: " << eocd.comment << endl;
  debugger.close();
}

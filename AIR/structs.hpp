#pragma once


struct CentralFileHeader{
  const quint32 cfh_signature = 0x02014b50;
  quint16 versionDone;
  quint16 neededVersion;
  quint16 flag;
  quint16 methodOfCompress;
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 CRC_32_uncompress;
  quint32 compressSize;
  quint32 nonCompressSize;
  quint16 sizeofNameFile;
  quint16 additionalSizeof;
  quint16 sizeofComment;
  quint16 numberOfDrive;
  quint16 internalAttributes;
  quint32 externalAttributes;
  quint32 offset;
  QString nameOfFile;
  QString comment;
};

/*
struct dataDescriptor{
  qint64 CRC_32;
  qint64 compressSize;
  qint64 unCompressSize;
};
*/

struct localFileHeader{
  const quint32 lfh_signature = 0x04034b50;
  quint16 neededVersion;
  quint16 flag;
  quint16 methodOfCompress;
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 CRC_32_uncompress;
  quint32 compressSize;
  quint32 nonCompressSize;
  quint16 sizeofNameFile;
  quint16 additionalSizeof;
  QString nameOfFile;
};

struct endOfCentralDirectory{
  const qint32 eocd_signature = 0x06054b50;
  qint16 numberOfDrive;
  qint16 numberOfDriveCFH;
  qint16 countOfCFH_onThisDrive;
  qint16 countOfCFH;
  quint32 sizeofCFH; // bytes
  quint32 offsetCFH_ofStartArchive;
  qint16 sizeofComment;
  QString comment;
};

class structs
{
public:
  struct CentralFileHeader *cfh;
  struct dataDescriptor *dd;
  struct localFileHeader *lfh;
  struct endOfCentralDirectory *eocd;

  void writeCFH(std::ofstream &file);

  //void writeDD(std::ofstream &file);

  void writeLFH(std::ofstream &file);

  void writeEOCD(std::ofstream &file);

  void writeLE(std::ofstream &file,quint16 data){
    quint8 bytes[2];
      bytes[0] = (data&0x000000FF);
      bytes[1] = (data&0x0000FF00)>>8;
      file.write(bytes[1],sizeof(char));
      file.write(bytes[0],sizeof(char));
  }

  void writeLE(std::ofstream &file,quint32 data){
    quint8 bytes[4];
      bytes[0] = (data&0x000000FF);
      bytes[1] = (data&0x0000FF00)>>8;
      bytes[2] = (data&0x0000FF00)>>8;
      bytes[3] = (data&0x0000FF00)>>8;
      file.write((char*)bytes[3],sizeof(char));
      file.write(bytes[2],sizeof(char));
      file.write(bytes[1],sizeof(char));
      file.write(bytes[0],sizeof(char));
  }
public:

  structs();

  ~structs();
};

